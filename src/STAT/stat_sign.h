#ifndef STAT_SIGN_H
#define STAT_SIGN_H

#include "stat_types.h"

int stat_sign_float(stat_float_t value);
int stat_sign_int32(int32_t value);
stat_float_t stat_copysign_float(stat_float_t magnitude, stat_float_t sign);

#endif // STAT_SIGN_H
