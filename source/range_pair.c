#include "range_pair.h"

double rangePair_getRange(struct RangePair pair) {
	return pair.max - pair.min;
}