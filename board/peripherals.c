#include "peripherals.h"
#include "MKL25Z4.h"

void enablePortA();
void enablePortE();

void BOARD_InitBootPeripherals(void) {
	enablePortA();
	enablePortE();
}

void enablePortA()
{
	SIM->SCGC5 |= 0x0200;
}

void enablePortE()
{
    SIM->SCGC5 |= 0x2000;
}
