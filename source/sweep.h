#ifndef sweep_H
#define sweep_H

#include "range_pair.h"
#include <stdint.h>

struct Sweep {
	struct RangePair range;
	int32_t position;
	uint32_t updateMillis;
	uint32_t lastMillis;
};

void sweep_setRange(struct Sweep* sweep, struct RangePair sweepRange);
void sweep_setUpdateMillis(struct Sweep* sweep, uint32_t updateMillis);
void sweep_setPeriod(struct Sweep* sweep, uint16_t millis);
void sweep_setPosition(struct Sweep* sweep, int32_t position);
void sweep_update(struct Sweep* sweep, uint32_t currentMillis);

#endif
