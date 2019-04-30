#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include "MKL25Z4.h"

struct ADC
{
	uint8_t channel;
};

void adc_init(struct ADC* adc, uint8_t channel);
void adc_convert(struct ADC* adc);
uint32_t adc_get();

#endif
