#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void adc_init();
void adc_startConversion();
uint32_t adc_get();
void adc_enablePortC2();

#endif
