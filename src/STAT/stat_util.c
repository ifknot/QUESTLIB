#include "stat_util.h"
#include <math.h>
#include <stdlib.h>

int stat_compare_floats(const void* a, const void* b) {
    stat_float_t fa = *(const stat_float_t*)a;
    stat_float_t fb = *(const stat_float_t*)b;
    return (fa > fb) - (fa < fb);
}

void stat_sort(stat_float_t* data, stat_size_t size) {
    qsort(data, size, sizeof(stat_float_t), stat_compare_floats);
}

bool stat_is_finite(stat_float_t value) {
    return isfinite(value);
}

bool stat_is_normal(stat_float_t value) {
    return isnormal(value);
}
