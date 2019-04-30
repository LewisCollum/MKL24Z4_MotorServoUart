#include "MKL25Z4.h"
#include "board.h"
#include "pwm.h"
#include "adc.h"
#include "mapper.h"
#include "range_pair.h"
#include "sweep.h"
#include "peripherals.h"

struct PWM servo;
struct PWM motor;
struct PWM buzzer;
struct ADC dial;
struct ADC light;

struct RangePair dialRange = {0,254};
struct RangePair lightRange = {50, 220};
struct RangePair servoDutyRange = {0.03, 0.125};
struct RangePair motorDutyRange = {0, 1};
struct RangePair buzzerFrequencyRange = {500, 2500};
struct Mapper mapper;
struct Sweep sweep;

void motor_init();
void servo_init();
void buzzer_init();
void dial_init();
void light_init();

int main() {

    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    buzzer_init();
    motor_init();
    servo_init();

    dial_init();
    light_init();
    sweep_init(&sweep, dialRange, dialRange.min, 8, 20);

    //int time = 0;
    while (1) {
    	//adc_convert(&dial);
    	adc_convert(&light);
    	double adcSample = (double)adc_get();
    	//sweep_update(&sweep, time);

    	//mapper_init(&mapper, dialRange, servoDutyRange);
    	mapper_init(&mapper, lightRange, servoDutyRange);
    	pwm_setDuty(&servo, mapper_map(&mapper, adcSample));
    	//pwm_setDuty(&servo, mapper_map(&mapper, sweep.pos));

    	//mapper_init(&mapper, dialRange, motorDutyRange);
    	mapper_init(&mapper, lightRange, motorDutyRange);
    	pwm_setDuty(&motor, mapper_map(&mapper, adcSample));

    	//mapper_init(&mapper, dialRange, buzzerFrequencyRange);
    	mapper_init(&mapper, lightRange, buzzerFrequencyRange);
    	pwm_setFrequency(&buzzer, mapper_map(&mapper, adcSample));
		pwm_setDuty(&buzzer, .5);
		//time++;
    }
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
