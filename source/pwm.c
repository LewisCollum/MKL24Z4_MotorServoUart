#include "pwm.h"

void setTimerClock(struct PWM* pwm);
void setPinMuxForTimer(struct PWM* pwm);
void setMode(struct PWM* pwm);
void setPwmOptions(struct PWM* pwm);

struct PwmPort
{
	PORT_Type* port;
	uint8_t pin;
};

const struct PwmPort pwmPorts[3] = {
		{PORTA, 4}, {PORTA, 13}, {PORTA, 2}
};

struct Timer
{
	uint32_t clockEnableOption;
	TPM_Type* timer;
};

const struct Timer timers[3] = {
		{0x01000000,TPM0}, {0x02000000,TPM1}, {0x04000000, TPM2}
};

enum PwmOptions {
	pwmoptions_pulsehigh = 0x20,
	pwmoptions_edgealigned = 0x08,
	pwmoptions_pinMux3 = 0x0300
};

struct Clock {
	uint32_t frequency;
	uint32_t selectOption;
};

const struct Clock clock48MHz = {.frequency = 48e6, .selectOption = 0x01000000};

void pwm_init(struct PWM* pwm, uint8_t timerChoice)
{
	pwm->timerChoice = timerChoice;
	setPinMuxForTimer(pwm);
	setTimerClock(pwm);
	enableTimerClock(pwm);
	setMode(pwm);
}

void setMode(struct PWM* pwm)
{
	pwm_disableTimer(pwm);
	setPwmOptions(pwm);
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
	pwm->timer->MOD = pwm->clockFrequency/pwm->frequency/pwm->prescaler;
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
	pwm->timer->SC |= 0x08;
}

void enableTimerClock(struct PWM* pwm)
{
	SIM->SCGC6 |= timers[pwm->timerChoice].clockEnableOption;
	pwm->timer = timers[pwm->timerChoice].timer;
}

void setTimerClock(struct PWM* pwm)
{
	pwm->clockFrequency = clock48MHz.frequency;
	SIM->SOPT2 |= clock48MHz.selectOption;
}

void setPinMuxForTimer(struct PWM* pwm)
{
	pwmPorts[pwm->timerChoice].port->PCR[pwmPorts[pwm->timerChoice].pin] = pwmoptions_pinMux3;
}

void setPwmOptions(struct PWM* pwm)
{
    pwm->timer->CONTROLS[1].CnSC = pwmoptions_pulsehigh | pwmoptions_edgealigned;
}
