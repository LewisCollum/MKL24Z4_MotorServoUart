#include "adc.h"

void readyConversion(struct ADC* adc);
void enableClockToADC();
void setModeSingleEnded();
void setChannelPinToAnalog(struct ADC* adc);
void setCFG1options();
void clearChannel();
void setSC3options();

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

enum SC3options {
		SC3options_contionuous = 0x08,
		SC3options_4bit = 0x04
};

enum CFG1options {
	CFG1options_divideRatioQuarter = 0x40,
	CFG1options_longSampleTime = 0x10,
	CFG1options_12BitConversion = 0x04,
	CFG1options_8BitConversionAndBusClock = 0x00,
	CFG1options_divideRatioHalf = 0x20
};

enum SC2options {
	SC2hardwareTrigger = 0x40,
	SC2softwareTrigger = ~SC2hardwareTrigger
};

void adc_init(struct ADC* adc, uint8_t channel)
{
	adc->channel = channel;

	enableClockToADC();
    ADC0->SC2 &= SC2softwareTrigger;
    setCFG1options();
    setChannelPinToAnalog(adc);
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
	setSC3options();
    clearChannel();
    ADC0->SC1[0] |= 0xF & channels[adc->channel];

}

void setModeSingleEnded()
{
	ADC0->SC1[0] &= ~0b10000;
}

void enableClockToADC()
{
	SIM->SCGC6 |= 0x8000000;
}

void setChannelPinToAnalog(struct ADC* adc)
{
	adcPorts[adc->channel].port->PCR[adcPorts[adc->channel].pin] = 0;
}

void setCFG1options()
{
    ADC0->CFG1 = CFG1options_divideRatioHalf | CFG1options_longSampleTime | CFG1options_8BitConversionAndBusClock;
}

void clearChannel()
{
	ADC0->SC1[0] &= ~0xF;

}

void setSC3options()
{
    ADC0->SC3 = SC3options_contionuous | SC3options_4bit;
}

