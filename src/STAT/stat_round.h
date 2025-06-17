#ifndef STAT_ROUND_H
#define STAT_ROUND_H

#include "stat_types.h"

int32_t stat_round_to_int32(stat_float_t value);
int32_t stat_floor_to_int32(stat_float_t value);
int32_t stat_ceil_to_int32(stat_float_t value);
int32_t stat_trunc_to_int32(stat_float_t value);

stat_float_t stat_round_to_multiple(stat_float_t value, stat_float_t multiple);
stat_float_t stat_round_decimal(stat_float_t value, int decimals);

bool stat_safe_round_to_int32(stat_float_t value, int32_t* result);

#endif // STAT_ROUND_H
