#include "stat_basic.h"
#include <math.h>
//#include <limits.h>

stat_float_t stat_min_float(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_float_t min = data[0];
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] < min) min = data[i];
    }
    return min;
}

int32_t stat_min_int32(int32_t* data, stat_size_t size) {
    if (size == 0) return INT32_MAX;

    int32_t min = data[0];
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] < min) min = data[i];
    }
    return min;
}

stat_float_t stat_max_float(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_float_t max = data[0];
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] > max) max = data[i];
    }
    return max;
}

int32_t stat_max_int32(int32_t* data, stat_size_t size) {
    if (size == 0) return INT32_MIN;

    int32_t max = data[0];
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] > max) max = data[i];
    }
    return max;
}

stat_float_t stat_range_float(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;
    return stat_max_float(data, size) - stat_min_float(data, size);
}

int32_t stat_range_int32(int32_t* data, stat_size_t size) {
    if (size == 0) return 0;
    return stat_max_int32(data, size) - stat_min_int32(data, size);
}

// uint32_t implementations
uint32_t stat_min_u32(uint32_t* data, stat_size_t size) {
    if (size == 0) return UINT32_MAX;
    uint32_t min = data[0];
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] < min) min = data[i];
    }
    return min;
}

uint32_t stat_max_u32(uint32_t* data, stat_size_t size) {
    if (size == 0) return 0;
    uint32_t max = data[0];
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] > max) max = data[i];
    }
    return max;
}

uint32_t stat_range_u32(uint32_t* data, stat_size_t size) {
    if (size == 0) return 0;
    return stat_max_u32(data, size) - stat_min_u32(data, size);
}
