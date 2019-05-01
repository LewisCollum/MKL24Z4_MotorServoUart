#include "MKL25Z4.h"
#include "board.h"
#include "pwm.h"
#include "adc.h"
#include "mapper.h"
#include "range_pair.h"
#include "sweep.h"
#include "peripherals.h"
#include "tick.h"
#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include <string.h>

void lcdInitDecorated() {
	lcdInit();
	lcdCommand(lcd4Bit | lcd2Line | lcdSkinny);
	lcdCommand(lcdOn);
	lcdCommand(lcdClear);
	lcdEnableClearAtColumnMax(16);
}

struct Sweep sweep;

struct PWM servo;
struct RangePair servoDutyRange = {0.03, 0.135};

struct PWM motor;
struct RangePair motorDutyRange = {0, 1};

struct PWM buzzer;
struct RangePair buzzerFrequencyRange = {500, 2500};

struct ADC dial;
struct RangePair dialRange = {0,254};

struct ADC light;
struct RangePair lightRange = {50, 220};

struct Mapper mapper;

struct Keypad keypad = {
	.portsOutput = {{PORTC, PTC, 0},{PORTC, PTC, 1},{PORTC, PTC, 3},{PORTC, PTC, 4}},
	.portsInput = {{PORTD, PTD, 0},{PORTD, PTD, 1},{PORTD, PTD, 2},{PORTD, PTD, 3}}
};


static volatile enum SystemState {systemState_waiting, systemState_selecting} systemState;

static volatile enum UartState {uartState_waiting, uartState_sending, uartState_fetching} uartState;
static volatile enum LcdState {lcdState_waiting, lcdState_selecting,
	lcdState_updateMode, lcdState_updateAngle} lcdState;
static volatile enum PwmState {pwmState_waiting, pwmState_dial, pwmState_light,
	pwmState_sweep, pwmState_servoUart, pwmState_motorUart} pwmState;

static volatile enum AdcState {adcState_waiting, adcState_reading, adcState_ready} adcState;
static volatile enum SweepState {sweepState_disabled, sweepState_enabled} sweepState;

static char keypadKey;
static char uartKey;
static uint8_t adcSample;

void motor_init();
void servo_init();
void buzzer_init();
void dial_init();
void light_init();
void tick_init();
void sweep_init();

void tick_handler();

int main() {
	__disable_irq();

	BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    buzzer_init();
    motor_init();
    servo_init();
    dial_init();
    light_init();
    tick_init();
    sweep_init();
    lcdInitDecorated();
    keypad_init(&keypad);
    uartInit();

    __enable_irq();

    while (1) {

    	switch(pwmState) {
    	case pwmState_waiting:
    		break;

    	case pwmState_dial:
    		mapper_init(&mapper, dialRange, servoDutyRange);
    		pwm_setDuty(&servo, mapper_map(&mapper, adcSample));

    		mapper_init(&mapper, dialRange, motorDutyRange);
    		pwm_setDuty(&motor, mapper_map(&mapper, adcSample));

			mapper_init(&mapper, dialRange, buzzerFrequencyRange);
			pwm_setFrequency(&buzzer, mapper_map(&mapper, adcSample));
			pwm_setDuty(&buzzer, .5);
    		break;

    	case pwmState_light:
    		mapper_init(&mapper, lightRange, servoDutyRange);
    		pwm_setDuty(&servo, mapper_map(&mapper, adcSample));

    		mapper_init(&mapper, lightRange, motorDutyRange);
    		pwm_setDuty(&motor, mapper_map(&mapper, adcSample));

			mapper_init(&mapper, lightRange, buzzerFrequencyRange);
			pwm_setFrequency(&buzzer, mapper_map(&mapper, adcSample));
			pwm_setDuty(&buzzer, .5);
    		break;

    	case pwmState_sweep:
    		pwm_setDuty(&buzzer, 0);
    		pwm_setDuty(&motor, motorDutyRange.min);
    		sweep_update(&sweep, tick_getCurrentMillis());
    		mapper_init(&mapper, sweep_getRange(&sweep), servoDutyRange);
    		pwm_setDuty(&servo, mapper_map(&mapper, sweep_getPosition(&sweep)));
    		break;

    	case pwmState_servoUart:
    		pwm_setDuty(&motor, motorDutyRange.min);
    		pwm_setDuty(&buzzer, 0);
    		break;

    	case pwmState_motorUart:
    		pwm_setDuty(&servo, servoDutyRange.min);
    		pwm_setDuty(&buzzer, 0);
    		break;

    	}

    	__disable_irq();
    	switch(lcdState) {
    	case lcdState_waiting:
    		break;

    	case lcdState_selecting:
    		lcdClearRow(0);
    		lcdClearRow(1);
    		char message[] = "Select [1-5]";
    		for (int i = 0; i <= 12; ++i) { //TODO add write string to LCD module
    			uartSend(message[i]);
    			while(uartIsSending());
    			lcdWriteDataToRow(message[i], 0);
    		}
    		lcdState = lcdState_waiting;
    		break;

    	case lcdState_updateMode:
    		lcdClearRow(0);
    		lcdClearRow(1);
    		char modeMessage[] = "Mode: ";
    		char keyCopy[1];
    		strcpy(keyCopy, &keypadKey);
    		strcat(modeMessage, keyCopy);
    		for (int i = 0; i <= 7; ++i) { //TODO add write string to LCD module
    			uartSend(modeMessage[i]);
    			while(uartIsSending());
    			lcdWriteDataToRow(modeMessage[i], 0);
    		}
    		lcdState = lcdState_waiting;
    		break;

    	case lcdState_updateAngle:
    		//char angle[4] =
    		break;
    	}
    	__enable_irq();

    	switch(adcState) {
    	case adcState_waiting:
    		break;

    	case adcState_reading:
    		switch(pwmState) {
    		case pwmState_dial:
    			adc_convert(&dial);
    			break;

    		case pwmState_light:
    			adc_convert(&light);
    			break;

    		default:
    			break;
    		}
    		break;

		case adcState_ready:
			adcSample = adc_get();
			adcState = adcState_waiting;
    		break;
    	}
    }
    return 0;
}

void tick_handler() {
	switch(adcState) {
	case adcState_waiting:
		adcState = adcState_reading;
		break;

	case adcState_reading:
		adcState = adcState_ready;
		break;

	default:
		break;
	}
}

void UART0_IRQHandler() {
	uartKey = UART0->D;
	lcdWriteDataToRow(uartKey, 1);
}

void PORTD_IRQHandler() {
	keypadKey = keypad_getPressedKey(&keypad);

	switch(systemState) {
	case systemState_waiting:
		if (keypadKey == '#') {
			systemState = systemState_selecting;
			lcdState = lcdState_selecting;
		}
		break;

	case systemState_selecting:
		lcdState = lcdState_selecting;
		switch(keypadKey) {
		case '1':
			pwmState = pwmState_dial;
			systemState = systemState_waiting;
			lcdState = lcdState_updateMode;
			break;
		case '2':
			pwmState = pwmState_light;
			systemState = systemState_waiting;
			lcdState = lcdState_updateMode;
			break;
		case '3':
			pwmState = pwmState_sweep;
			systemState = systemState_waiting;
			lcdState = lcdState_updateMode;
			break;
		case '4':
			pwmState = pwmState_servoUart;
			systemState = systemState_waiting;
			lcdState = lcdState_updateMode;
			break;
		case '5':
			pwmState = pwmState_motorUart;
			systemState = systemState_waiting;
			lcdState = lcdState_updateMode;
			break;
		default: break;
		}
		break;

	default:
		break;
	}
	PORTD->ISFR = 0xF;
}

void buzzer_init()
{
	pwm_init(&buzzer, 2);
   	pwm_setPrescaler(&buzzer, 16);
   	pwm_setDuty(&buzzer, .5);
   	pwm_enableTimer(&buzzer);
}

void motor_init()
{
	pwm_init(&motor, 1);
	pwm_setPrescaler(&motor, 8);
	pwm_setFrequency(&motor, 2400);
	pwm_enableTimer(&motor);
}

void servo_init()
{
	pwm_init(&servo, 0);
   	pwm_setPrescaler(&servo, 16);
   	pwm_setFrequency(&servo, 50);
   	pwm_enableTimer(&servo);
}

void light_init()
{
	adc_init(&light, 1);
}

void dial_init()
{
	adc_init(&dial, 0);
}

void tick_init()
{
    tick_setHandler(tick_handler);
    tick_run(1);
}

void sweep_init()
{
    sweep_setUpdateMillis(&sweep, tick_getUpdateMillis());
    sweep_setPeriod(&sweep, 800);
    sweep_setPosition(&sweep, sweep_getRange(&sweep).min);
}
