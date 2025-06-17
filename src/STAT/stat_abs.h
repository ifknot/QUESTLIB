#ifndef STAT_ABS_H
#define STAT_ABS_H

#include "stat_types.h"

stat_float_t stat_abs_float(stat_float_t value);
int32_t stat_abs_int32(int32_t value);
bool stat_safe_abs_int32(int32_t value, int32_t* result);

#endif // STAT_ABS_H
