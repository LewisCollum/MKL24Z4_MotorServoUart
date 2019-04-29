#include "adc.h"
#include "MKL25Z4.h"
#define dcmotorMod 2047

void adc_init()
{
    SIM->SCGC5 |= 0x2000;       /* clock to PORTE */
    PORTE->PCR[20] = 0;         /* PTE20 analog input */

    SIM->SCGC6 |= 0x8000000;    /* clock to ADC0 */
    ADC0->SC2 &= ~0x40;         /* software trigger */
    ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
}

void adc_startConversion()
{
	ADC0->SC1[0] = 0;           /* start conversion on channel 0 */
	while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
}

uint32_t adc_get()
{
	return ADC0->R[0];        /* read conversion result and clear COCO flag */
}

