#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>
#include "MKL25Z4.h"

struct PWM {
	uint8_t timerChoice;
	TPM_Type* timer;
	uint16_t frequency;
	uint16_t prescaler;
	uint32_t clockFrequency;
	float duty;
};

void pwm_enableClock(struct PWM* pwm);
void pwm_init(struct PWM* pwm, uint8_t timerChoice);
void pwm_setMode(struct PWM* pwm);
void pwm_setPrescaler(struct PWM* pwm, uint16_t prescaler);
void pwm_setFrequency(struct PWM* pwm, uint16_t frequency);
void pwm_setDuty(struct PWM* pwm, float dutyPercent);
void pwm_disableTimer(struct PWM* pwm);
void pwm_enableTimer(struct PWM* pwm);

#endif
