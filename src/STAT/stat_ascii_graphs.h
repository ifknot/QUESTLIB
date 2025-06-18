#ifndef stat_ASCII_GRAPHS_H
#define stat_ASCII_GRAPHS_H

#include <stdio.h>
#include <assert.h>

#include "stat_types.h"
#include "stat_basic.h"

void stat_histogram(const stat_size_t* values, stat_size_t count, stat_size_t max_width) {
    assert(count && "ZERO values!");
    assert(values && "NULL values!");
    assert(max_width && "ZERO max width!");

    stat_size_t max_val = stat_max_u32((uint32_t*)values, count);

    printf("\nDistribution:\n");

    for (stat_size_t i = 0; i < count; i++) {
        // Calculate bar length (scaled to max_width)
        stat_size_t bar_length = (stat_size_t)((uint64_t)values[i] * max_width / max_val);

        printf("%3zu ", i);

        // Draw the bar
        for (stat_size_t j = 0; j < bar_length; j++) {
            putchar('=');
        }

        // Show exact value at end
        printf(" (%u)", values[i]); // TODO make this optional

        putchar('\n');
    }
}

#endif
