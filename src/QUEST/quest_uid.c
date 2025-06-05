/**
 * @file quest_uid.c
 * @brief Unique ID Generator Implementation
 *
 * Implements a simple sequential unique ID generator.
 * The internal counter starts at 0 and increments with each call.
 */
#include "quest_uid.h"
#include "quest_types.h"

/**
 * @brief Internal state structure for the UID generator
 *
 * Contains the current counter value that gets incremented
 * on each call to quest_next_uid().
 */
struct quest_uid_t {
    quest_size_t i;  /**< Current counter value */
};

/**
 * @brief Static instance of the UID generator state
 *
 * Initialized to zero at program startup. 
 * Maintains persistence between calls to quest_next_uid().
 */
static struct quest_uid_t uid = { 0 };

/**
 * @brief Implementation of quest_next_uid()
 *
 * @return The next available unique identifier
 *
 * @details
 * This implementation simply increments and returns a counter.
 * @note The counter starts at 0 
 * @warning Counter will wrap around when reaching the maximum value of quest_size_t.
 */
quest_size_t quest_next_uid(void) {
    return uid.i++;
}
