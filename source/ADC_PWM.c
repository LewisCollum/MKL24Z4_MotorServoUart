#include "MKL25Z4.h"
#include "board.h"
#include "pwm.h"
#include "adc.h"

struct PWM servo;

int main() {
//	servo->timer = TPM0;
//	servo->frequency = 50;
//	servo->prescaler = 16;
//	servo->duty = 0;

    BOARD_InitBootClocks();
    adc_init();

    pwm_init(&servo);
    pwm_setFrequency(&servo, 50);
    pwm_setPrescaler(&servo, 16);

    while (1) {
    	adc_startConversion();
    	float slope = (0.125-0.035) / (4095-0);
    	float adcVal = (float)adc_get();
    	float mappedValue = 0.035 + slope*(adcVal);
    	pwm_setDuty(&servo, mappedValue);
    }
}






