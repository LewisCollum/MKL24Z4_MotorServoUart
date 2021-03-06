#include "sweep.h"
#include <stdint.h>

static int32_t increment = 1;

void sweep_setPeriod(struct Sweep* sweep, uint16_t millis) {
	sweep->range = (struct RangePair){0, millis/sweep->updateMillis};
}

const struct RangePair sweep_getRange(struct Sweep* sweep) {
	return sweep->range;
}

void sweep_setUpdateMillis(struct Sweep* sweep, uint32_t updateMillis) {
	sweep->updateMillis = updateMillis;
}

void sweep_setPosition(struct Sweep* sweep, int32_t position) {
	if (position < sweep->range.min)
		sweep->position = sweep->range.min;
	else if (position > sweep->range.max)
		sweep->position = sweep->range.max;
	else
		sweep->position = position;
}

const int32_t sweep_getPosition(struct Sweep* sweep) {
	return sweep->position;
}

void updateIncrement(struct Sweep* sweep) {
	if(sweep->position == sweep->range.max || sweep->position == sweep->range.min)
		increment = -increment;
}

void sweep_update(struct Sweep* sweep, uint32_t currentMillis){
	if(currentMillis - sweep->lastMillis >= sweep->updateMillis) {
		sweep_setPosition(sweep, sweep->position + increment);
		updateIncrement(sweep);
		sweep->lastMillis = currentMillis;
	}
}
