#ifndef mapper_H
#define mapper_h

#include "range_pair.h"
#include <stdint.h>

struct Mapper {
	struct RangePair input;
	struct RangePair output;
	double slope;
};

void mapper_init(struct Mapper* mapper, struct RangePair input, struct RangePair output);
double mapper_map(struct Mapper* mapper, double value);

#endif
