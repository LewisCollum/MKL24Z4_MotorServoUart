#include "adc.h"

void readyConversion(struct ADC* adc);
void enableClock();
void setModeSingleEnded();

struct AdcPort
{
	PORT_Type* port;
	uint8_t pin;
};

const struct AdcPort adcPorts[14] = {
	{PORTE,20}, {PORTE,22}, {PORTE,21}
};

const uint8_t channels[14] = {
	0,3,4,5,6,7,8,9,11,12,13,14,15,23
};

void adc_init(struct ADC* adc, uint8_t channel)
{
	adc->channel = channel;

	enableClock();
    ADC0->SC2 &= ~0x40;         /* software trigger */
    ADC0->CFG1 = adcoptions_divideRatioQuarter | adcoptions_longSampleTime | adcoptions_12BitConversion | adcoptions_busClock;
//    ADC0->CFG1 = 0b0010001;

	adcPorts[channel].port->PCR[adcPorts[channel].pin] = 0;
	setModeSingleEnded();
}

void adc_convert(struct ADC* adc)
{
	readyConversion(adc);
	while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
}

uint32_t adc_get()
{
	return ADC0->R[0];        /* read conversion result and clear COCO flag */
}

void readyConversion(struct ADC* adc)
{
    ADC0->SC3 = adcoptions_contionuous | adcoptions_4bit;
	ADC0->SC1[0] &= ~0xF;
    ADC0->SC1[0] |= 0xF & channels[adc->channel];

}

void setModeSingleEnded()
{
	ADC0->SC1[0] &= ~0b10000;
}

void enableClock()
{
	SIM->SCGC6 |= 0x8000000;
}

