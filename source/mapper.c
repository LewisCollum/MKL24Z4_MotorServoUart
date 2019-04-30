#include "mapper.h"
#include <stdint.h>
#include <stdio.h>

void clipMappingAtOutputMin(struct Mapper* mapper);
void clipMappingAtOutputMax(struct Mapper* mapper);

static double mapping, valueToMap;

void mapper_init(struct Mapper* mapper, struct RangePair input, struct RangePair output) {
	mapper->input = input;
	mapper->output = output;
	mapper->slope = rangePair_getRange(mapper->output) / rangePair_getRange(mapper->input);
}

double mapper_map(struct Mapper* mapper, double value)
{
	valueToMap = value;
	mapping = mapper->output.min + mapper->slope * (valueToMap - mapper->input.min);
	clipMappingAtOutputMin(mapper);
	clipMappingAtOutputMax(mapper);
	return mapping;
} 

void clipMappingAtOutputMin(struct Mapper* mapper) {
	if(mapping < mapper->output.min) mapping = mapper->output.min;
}

void clipMappingAtOutputMax(struct Mapper* mapper) {
	if(mapping > mapper->output.max) mapping = mapper->output.max;
}
