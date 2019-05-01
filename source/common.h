#ifndef COMMON_H_
#define COMMON_H_

#define true 1
#define false 0

enum Pin {
	pin_gpio = 0x100,
	pin_pullup = 0x3,
	pin_interrupt = 0xA0000
};

enum GpioDirection {
	gpio_input,
	gpio_output
};

#define clockPortC 0x0800
#define clockPortD 0x1000
#define clockPortE 0x2000

#endif
