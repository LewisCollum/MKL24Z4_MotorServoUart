#ifndef range_pair_H
#define range_pair_H

struct RangePair {
	double min;
	double max;
};

double rangePair_getRange(struct RangePair rangePair);

#endif