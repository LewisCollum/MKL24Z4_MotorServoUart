#ifndef tick_H_
#define tick_H_

#include "stdint.h"

typedef void (*TickHandler)();

void tick_run(uint16_t millis);
void tick_setHandler(TickHandler handler);
uint32_t tick_getCurrentMillis();
uint32_t tick_getUpdateMillis();

#endif
