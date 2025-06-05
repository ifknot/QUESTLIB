/**
 * @file quest_uid.h
 * @brief Unique ID Generator Interface
 *
 * Provides functionality to generate unique sequential identifiers.
 * Uses an opaque structure to maintain internal state.
 */
#ifndef QUEST_UID_H
#define QUEST_UID_H

#include <stdint.h>

typedef uint16_t quest_uid_size_t;  // upto 65535 unique IDs places a ceiling on number of objects in a quest game

/**
 * @brief Opaque handle for the UID generator state
 *
 * This structure maintains the internal state of the UID generator.
 * Its actual definition is hidden in the implementation file.
 */
typedef struct quest_uid_t quest_uid_t;

/**
 * @brief Generates the next unique identifier in sequence
 * @return The next available unique identifier (quest_size_t)
 */
quest_uid_size_t quest_next_uid();

#endif
