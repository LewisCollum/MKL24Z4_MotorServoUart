#include "pwm.h"

void setTimer(struct PWM* pwm);

void pwm_init(struct PWM* pwm, uint8_t timerChoice)
{
	pwm->timerChoice = timerChoice;
	pwm_enableClock(pwm);
	setTimer(pwm);
}

void pwm_setMode(struct PWM* pwm)
{
	pwm_disableTimer(pwm);
    pwm->timer->CONTROLS[1].CnSC = pwm_pulsehigh | edge_aligned;
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

void setTimer(struct PWM* pwm)
{
	switch(pwm->timerChoice) {
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

void pwm_enableClock(struct PWM* pwm)
{
	switch(pwm->timerChoice) {
	case 0:
		SIM->SCGC6 |= 0x01000000;
		break;
	case 1:
		SIM->SCGC6 |= 0x02000000;
		break;
	case 2:
		SIM->SCGC6 |= 0x04000000;
		break;
	}
	   		   /* enable clock to TPM0 */
}
