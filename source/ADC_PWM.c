#include "MKL25Z4.h"
#include "board.h"
#include "pwm.h"
#include "adc.h"
#include "mapper.h"
#include "range_pair.h"

struct PWM servo;
struct PWM motor;
struct PWM buzzer;
struct ADC dial;
struct ADC light;

struct RangePair dialRange = {0,4095};
struct RangePair lightRange = {0, 4095};
struct RangePair servoDutyRange = {0.035, 0.125};
struct RangePair motorDutyRange = {0, 1};
struct RangePair buzzerFrequencyRange = {500, 2500};
struct Mapper mapper;

void motor_init();
void servo_init();
void buzzer_init();
void dial_init();
void light_init();

int main() {

    BOARD_InitBootClocks();

    buzzer_init();
    motor_init();
    servo_init();

    SIM->SCGC5 |= 0x2000; //enable clock port e

    dial_init();
    light_init();



    while (1) {
    	adc_convert(&dial);
    	//adc_convert(&light);
    	double adcSample = (double)adc_get();

    	mapper_init(&mapper, dialRange, servoDutyRange);
    	pwm_setDuty(&servo, mapper_map(&mapper, adcSample));

    	mapper_init(&mapper, dialRange, motorDutyRange);
    	pwm_setDuty(&motor, mapper_map(&mapper, adcSample));

    	mapper_init(&mapper, dialRange, buzzerFrequencyRange);
    	pwm_setFrequency(&buzzer, mapper_map(&mapper, adcSample));
		pwm_setDuty(&buzzer, .5);
    }
}

void buzzer_init()
{
	pwm_init(&buzzer, 2);
    pwm_setMode(&buzzer);
   	pwm_setPrescaler(&buzzer, 16);
   	pwm_setDuty(&buzzer, .5);
   	pwm_enableTimer(&buzzer);
}

void motor_init()
{
	pwm_init(&motor, 1);
	pwm_setMode(&motor);
	pwm_setPrescaler(&motor, 8);
	pwm_setFrequency(&motor, 2400);
	pwm_enableTimer(&motor);
}

void servo_init()
{
	pwm_init(&servo, 0);
    pwm_setMode(&servo);
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





