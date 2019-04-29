#include "MKL25Z4.h"
#include "board.h"
#include "pwm.h"
#include "adc.h"

struct PWM servo;
struct PWM motor;

void motor_init();
void servo_init();

int main() {

    BOARD_InitBootClocks();

    motor_init();
    servo_init();

    adc_init();


    while (1) {
    	adc_startConversion();
    	float slope = (0.125-0.035) / (4095-0);
    	float adcVal = (float)adc_get();
    	float mappedValue = 0.035 + slope*(adcVal);
    	pwm_setDuty(&servo, mappedValue);

    	float slope2 = (1.0-0.0) / (4095-0);
		float mappedValue2 = 0 + slope2*(adcVal);
    	pwm_setDuty(&motor, mappedValue2);
    }
}

void motor_init()
{
	pwm_init(&motor, 1);
	pwm_setMode(&motor);
	pwm_setPrescaler(&motor, 16);
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






