#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>
#include "MKL25Z4.h"

enum qwerty {
	pwm_pulsehigh = 0x20,
	edge_aligned = 0x08
};

struct PWM {
	TPM_Type* timer;
	uint16_t frequency;
	uint16_t prescaler;
	float duty;
};

void pwm_init(struct PWM* pwm);
void pwm_setPrescaler(struct PWM* pwm, uint16_t prescaler);
void pwm_setFrequency(struct PWM* pwm, uint16_t frequency);
void pwm_setDuty(struct PWM* pwm, float dutyPercent);
void pwm_disableTimer(struct PWM* pwm);
void pwm_enableTimer(struct PWM* pwm);
void pwm_setTimer(struct PWM* pwm, uint8_t timerChoice);


#endif
