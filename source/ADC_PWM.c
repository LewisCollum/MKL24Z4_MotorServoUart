#include "MKL25Z4.h"
#include "board.h"
#include "pwm.h"
#include "adc.h"
#include "mapper.h"
#include "range_pair.h"
#include "sweep.h"
#include "peripherals.h"
#include "tick.h"

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

    __enable_irq();

    while (1) {
    	//adc_convert(&dial);
    	//adc_convert(&light);
    	//double adcSample = (double)adc_get();
    	//sweep_update(&sweep, time);
    	sweep_update(&sweep, tick_getCurrentMillis());

    	//mapper_init(&mapper, dialRange, servoDutyRange);
    	//mapper_init(&mapper, lightRange, servoDutyRange);
    	mapper_init(&mapper, sweep.range, servoDutyRange);
    	//pwm_setDuty(&servo, mapper_map(&mapper, adcSample));
    	pwm_setDuty(&servo, mapper_map(&mapper, sweep.position));

    	//mapper_init(&mapper, dialRange, motorDutyRange);
    	mapper_init(&mapper, lightRange, motorDutyRange);
    	//pwm_setDuty(&motor, mapper_map(&mapper, adcSample));

    	//mapper_init(&mapper, dialRange, buzzerFrequencyRange);
    	mapper_init(&mapper, lightRange, buzzerFrequencyRange);
    	//pwm_setFrequency(&buzzer, mapper_map(&mapper, adcSample));
		pwm_setDuty(&buzzer, .5);
    }
    return 0;
}

void tick_handler(){}

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
    sweep_setPosition(&sweep, sweep.range.min);
}
