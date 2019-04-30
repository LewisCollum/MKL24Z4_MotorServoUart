#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include "MKL25Z4.h"

struct ADC
{
	uint8_t channel;
};

enum adcoptions {
		adcoptions_contionuous = 0x08,
		adcoptions_4bit = 0x04,
		adcoptions_divideRatioQuarter = 0x40,
		adcoptions_longSampleTime = 0x10,
		adcoptions_12BitConversion = 0x04,
		adcoptions_busClock = 0x00
};
void adc_init(struct ADC* adc, uint8_t channel);
void adc_convert(struct ADC* adc);
uint32_t adc_get();

#endif
