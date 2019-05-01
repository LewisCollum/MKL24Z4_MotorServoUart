#include "peripherals.h"
#include "MKL25Z4.h"

void enablePortA()
{
	SIM->SCGC5 |= 0x0200;
}

void enablePortC()
{
	SIM->SCGC5 |= 0x0800;
}

void enablePortD()
{
    SIM->SCGC5 |= 0x1000;
}

void enablePortE()
{
    SIM->SCGC5 |= 0x2000;
}

void BOARD_InitBootPeripherals(void) {
	enablePortA();
	enablePortC();
	enablePortD();
	enablePortE();
}

