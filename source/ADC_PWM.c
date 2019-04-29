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

	SIM->SCGC5 |= 0x1800;       		   /* enable clock to Port C*/
	pwm_init(&servo, 0);
	PORTC->PCR[2] = 0x0400;     		   /* PTC2 used by TPM0 */
    SIM->SOPT2 |= 0x01000000;   		   /* use MCGFLLCLK as timer counter clock */


    adc_init();

    pwm_setMode(&servo);
	pwm_setFrequency(&servo, 50);
	pwm_setPrescaler(&servo, 16);
	pwm_enableTimer(&servo);
    //motor_init();
    //servo_init();

    while (1) {
    	adc_startConversion();
    	float slope = (0.125-0.035) / (4095-0);
    	float adcVal = (float)adc_get();
    	float mappedValue = 0.035 + slope*(adcVal);
    	pwm_setDuty(&servo, mappedValue);
    }
}

void motor_init()
{
//	setTimer(&motor, 0);
//	pwm_setMode(&motor);
//	pwm_setFrequency(&motor, .024);
//	pwm_setPrescaler(&motor, 16);
//	pwm_enableTimer(&motor);
}

void servo_init()
{

}






