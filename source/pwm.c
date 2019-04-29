#include "pwm.h"

void pwm_init(struct PWM* pwm)
{
	pwm_setTimer(pwm, 0);
	pwm_disableTimer(pwm);
    pwm->timer->CONTROLS[1].CnSC = pwm_pulsehigh | edge_aligned;

    pwm_setPrescaler(pwm, 16);
    pwm_enableTimer(pwm);
    pwm_setFrequency(pwm, 50);
}

void pwm_setPrescaler(struct PWM* pwm, uint16_t prescaler) {
	pwm->prescaler = prescaler;
	int shiftCount = 0;
	while(prescaler>>=1){
		++shiftCount;
	}
	pwm->timer->SC |= shiftCount;
}

void pwm_setFrequency(struct PWM* pwm, uint16_t frequency)
{
	pwm->frequency = frequency;
	pwm->timer->MOD = 48e6/pwm->frequency/pwm->prescaler;
}

void pwm_setDuty(struct PWM* pwm, float dutyPercent)
{
	pwm->duty = dutyPercent;
    pwm->timer->CONTROLS[1].CnV = pwm->timer->MOD*pwm->duty;
}

void pwm_disableTimer(struct PWM* pwm)
{
	pwm->timer->SC = 0;
}

void pwm_enableTimer(struct PWM* pwm)
{
	pwm->timer->SC |= 0b1000;
}

void pwm_setTimer(struct PWM* pwm, uint8_t timerChoice)
{
	switch(timerChoice) {

	case 0 :
		pwm->timer = TPM0;
		break;
	case 1 :
		pwm->timer = TPM1;
		break;
	case 2 :
		pwm->timer = TPM2;
		break;

	}
}
