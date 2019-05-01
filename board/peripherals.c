#include "peripherals.h"
#include "MKL25Z4.h"

void enablePortA();
void enablePortC();
void enablePortE();

void BOARD_InitBootPeripherals(void) {
	enablePortA();
	enablePortC();
	enablePortE();
}

void enablePortA()
{
	SIM->SCGC5 |= 0x0200;
}

void enablePortC()
{
	SIM->SCGC5 |= 0x0800;
}

void enablePortE()
{
    SIM->SCGC5 |= 0x2000;
}
