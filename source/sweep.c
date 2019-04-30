#include "sweep.h"
#include <stdint.h>

void sweep_init(struct Sweep* sweep, struct RangePair pair, uint32_t curPos, uint32_t incrementSize, uint32_t updateInterval){
	sweep->pos = curPos;
	sweep->increment = incrementSize;
	sweep->updateInterval = updateInterval;
	sweep->lastUpdate = 0;
	sweep->rangePair = pair;
}

void sweep_update(struct Sweep* sweep, uint32_t millis){
	if((millis - sweep->lastUpdate) >= sweep->updateInterval)
	{
		sweep->lastUpdate = millis;
		sweep->pos += sweep->increment;
		if((sweep->pos >= sweep->rangePair.max) || (sweep->pos <= sweep->rangePair.min))
		{
			sweep->increment = -sweep->increment;
		}
	}
}
