#ifndef sweep_H
#define sweep_H

#include "range_pair.h"
#include <stdint.h>

struct Sweep {
	struct RangePair range;
	uint32_t pos;
	uint32_t increment;
	uint32_t updateInterval;
	uint32_t lastUpdate;
	struct RangePair rangePair;

};

void sweep_init(struct Sweep* sweep, struct RangePair pair, uint32_t curPos, uint32_t incrementSize, uint32_t updateInterval);
void sweep_update(struct Sweep* sweep, uint32_t millis);

#endif