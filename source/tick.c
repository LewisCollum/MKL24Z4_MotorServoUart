#include "tick.h"
#include "MKL25Z4.h"

struct Tick{
	const uint32_t clockFreq;
	TickHandler handler;
	uint16_t updateMillis;
	volatile uint32_t currentMillis;
};

enum controlOptions {
	control_enableTick = 0b1,
	control_enableInterrupt = 0b10,
	control_useSystemClock = 0b100,
};

static struct Tick tick = {.clockFreq = 48000000};


void tick_setHandler(TickHandler handler){
	tick.handler = handler;
}

void tick_run(uint16_t millis){
	tick.updateMillis = millis;
	SysTick->CTRL = control_enableInterrupt | control_enableTick;

	uint32_t clockCycles = (tick.clockFreq/1000/16)*millis;
	SysTick->LOAD = clockCycles;
}

void SysTick_Handler(){
	tick.handler();
	tick.currentMillis += tick.updateMillis;
}

uint32_t tick_getUpdateMillis(){
	return tick.updateMillis;
}

uint32_t tick_getCurrentMillis(){
	return tick.currentMillis;
}
