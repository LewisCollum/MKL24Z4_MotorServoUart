#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "mkl25z4.h"

#define keypad_portSize 4

struct KeypadPort {
	PORT_Type* port;
	GPIO_Type* gpio;
	uint8_t pin;
};

struct Keypad {
	const struct KeypadPort portsOutput[keypad_portSize];
	const struct KeypadPort portsInput[keypad_portSize];
	uint8_t column;
	uint8_t row;
	uint8_t keypressDetected;
};

void keypad_init(struct Keypad* keypad);
void keypad_scan(struct Keypad* keypad);
char keypad_getPressedKey(struct Keypad* keypad);

#endif
