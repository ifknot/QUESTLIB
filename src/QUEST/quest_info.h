#ifndef QUEST_INFO_H
#define QUEST_INFO_H

#include <assert.h>

#include "../MEM/mem_arena.h"
#include "../STR/str_memory.h"

/**
 * @brief Dual-description container for game locations
 *
 * Contains both short and long form descriptions for:
 * - Room/area titles (brief)
 * - Detailed environmental descriptions (details)
 *
 * @note Both strings should be null-terminated UTF-8.
 * @warning Memory management policy:
 * - Allocated by location loader
 * - Freed by location unloader
 * - Never modified after creation
 */
typedef struct {
    char* brief;    /**< Short identifier (e.g., "Dark Forest Entrance")
                        - Max 255 chars recommended */
    char* details;  /**< Full description (e.g., "The twisted oaks creak...")
                        - Supports multi-paragraph text */
} quest_info_t;

/**
 * @brief Creates info with existing string buffers (no copying)
 * @param arena Memory arena for allocation
 * @param string_view_brief Pre-allocated brief string
 * @param string_view_details Pre-allocated details string
 * @return New info struct or NULL
 *
 * @warning Does NOT copy strings - lifetime must match arena
 *
 * @code
 * // For static/read-only strings:
 * quest_info_t* static_info = quest_info_link(
 *     arena,
 *     "Static Title",  // Will not be modified/freed
 *     "Static description"
 * );
 * @endcode
 */
quest_info_t* quest_info_link(
    mem_arena_t* arena,
    char* string_view_brief,
    char* string_view_details
);

/**
 * @brief Creates info with empty string buffers of specified sizes
 * @param arena Memory arena for allocation
 * @param size_brief Maximum brief string length (including null)
 * @param size_details Maximum details string length (including null)
 * @return New info struct or NULL
 *
 * @code
 * // For mutable strings:
 * quest_info_t* dynamic_info = quest_info_create(arena, 32, 256);
 * str_copy(dynamic_info->brief, "Dynamic Title");
 * str_append(dynamic_info->details, "Mutable description...");
 * @endcode
 */
quest_info_t* quest_info_create(
    mem_arena_t* arena,
    str_size_t size_brief,
    str_size_t size_details
);

/**
 * @brief Creates info with copied strings
 * @param arena Memory arena for allocation
 * @param string_brief String to copy for brief
 * @param string_details String to copy for details
 * @return New info struct or NULL
 *
 * @code
 * // For string literals/stack strings:
 * quest_info_t* copied_info = quest_info_copy(
 *     arena,
 *     "Copied Title",
 *     "This description will be arena-allocated"
 * );
 * @endcode
 */
quest_info_t* quest_info_copy(
    mem_arena_t* arena,
    const char* string_brief,
    const char* string_details
);

#endif
