#include "keypad.h"
#include "MKL25Z4.h"
#include "delay.h"
#include "common.h"

const char hexKeys[] = {
		'1', '2', '3', 'A',
		'4', '5', '6', 'B',
		'7', '8', '9', 'C',
		'*', '0', '#', 'D'
};

void setOutputLow(struct Keypad* keypad);
void setOutputHigh(struct Keypad* keypad);
void setRowLow(struct Keypad* keypad);

void keypad_init(struct Keypad* keypad) {
	for (int i = 0; i < keypad_portSize; ++i) {
		keypad->portsOutput[i].port->PCR[keypad->portsOutput[i].pin] = pin_gpio | pin_pullup;
		keypad->portsOutput[i].gpio->PDDR |= 1 << keypad->portsOutput[i].pin;

		keypad->portsInput[i].port->PCR[keypad->portsInput[i].pin] = pin_gpio | pin_pullup | pin_interrupt;
		keypad->portsInput[i].gpio->PDDR &= ~(1 << keypad->portsInput[i].pin);
	}

	setOutputLow(keypad);
	NVIC_EnableIRQ(PORTD_IRQn);
	NVIC_SetPriority(PORTD_IRQn, 3);
}

void setOutputLow(struct Keypad* keypad) {
	for(int i = 0; i < keypad_portSize; ++i)
		keypad->portsOutput[i].gpio->PCOR |= 1 << keypad->portsOutput[i].pin;
}

void setOutputHigh(struct Keypad* keypad) {
	for(int i = 0; i < keypad_portSize; ++i)
		keypad->portsOutput[i].gpio->PDOR |= 1 << keypad->portsOutput[i].pin;
}

void setRowLow(struct Keypad* keypad) {
	keypad->portsOutput[keypad->row].gpio->PCOR |= 1 << keypad->portsOutput[keypad->row].pin;
}

void keypad_scan(struct Keypad* keypad) {
	for (keypad->row = 0; keypad->row < keypad_portSize; ++keypad->row) {
		setOutputHigh(keypad);
		setRowLow(keypad);

		delayUs(5);
		for (keypad->column = 0; keypad->column < keypad_portSize; ++keypad->column) {
			if (~keypad->portsInput[keypad->column].gpio->PDIR & 1 << keypad->portsInput[keypad->column].pin) {
				keypad->keypressDetected = true;
				return;
			}
		}
	}
}

char keypad_getPressedKey(struct Keypad* keypad) {
	keypad_scan(keypad);
	setOutputLow(keypad);
	if (keypad->keypressDetected) {
		keypad->keypressDetected = false;
		return hexKeys[keypad->row * 4 + keypad->column];
	}
	else return 0;
}
