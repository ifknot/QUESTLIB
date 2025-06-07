/**
 * @file mem_arena.h
 * @brief Linear memory allocator for deterministic lifetime management
 * @defgroup memory_arena Memory Arena
 * @{
 */
#ifndef MEM_ARENA_H
#define MEM_ARENA_H

#include <stdio.h>

#include "mem_constants.h"
#include "mem_types.h"

/* ----------------- Arena Policies ----------------- */

/**
 * @brief Memory allocation strategy options
 * @dot
 * digraph policies {
 *     node [shape=box, fontname="Courier New"];
 *     DOS [label="DOS Policy\n(INT 21h allocations)"];
 *     C [label="C Policy\n(malloc/free backend)"];
 * }
 * @enddot
 */
typedef enum {
  MEM_ARENA_POLICY_DOS,
  MEM_ARENA_POLICY_C
} mem_arena_policy_t;

/// Human-readable policy names
static const char mem_policy_info[2][31] = {
	 "MEM_POLICY_DOS",
	 "MEM_POLICY_C"
};

/* ----------------- Arena Structure ----------------- */

/**
 * @brief Opaque memory arena handle
 * @dot
 * digraph arena {
 *     node [shape=record, fontname="Courier New"];
 *     arena [label="<f0> Policy|<f1> Base Ptr|<f2> Used|<f3> Capacity|<f4> MCB Ptr"];
 * }
 * @enddot
 *
 * @warning Contents are private - use accessor functions
 */
typedef struct private_mem_arena_t mem_arena_t;

/* ----------------- Core Operations ----------------- */

/**
 * @brief Creates a new memory arena
 * @param policy Allocation strategy (DOS/C)
 * @param byte_request Initial size in bytes
 * @return Arena handle or NULL on failure
 *
 * @details DOS Version Compatibility:
 * @code
 * | Feature       | DOS 2.0 | DOS 3.0+ |
 * |---------------|---------|----------|
 * | Arena Creation|   Yes   |   Yes    |
 * | >64KB Arenas  |   No    |   Yes    |
 * | MCB Chain     | Partial |   Full   |
 * @endcode
 *
 * @note For DOS policy, maximum initial size is 65535 paragraphs (≈1MB)
 * @see mem_arena_delete()
 */
mem_arena_t* mem_arena_create(mem_arena_policy_t policy, mem_size_t byte_request);

/**
 * @brief Destroys an arena and all its allocations
 * @param arena Valid arena handle
 * @return Bytes freed (0 if arena was NULL)
 *
 * @warning All pointers from this arena become invalid
 */
mem_size_t mem_arena_delete(mem_arena_t* arena);

/* ----------------- Accessors ----------------- */

/**
 * @brief Gets DOS Memory Control Block for arena
 * @param arena Arena created with DOS policy
 * @return MCB pointer or NULL if C policy
 *
 * @pre arena != NULL
 * @see mem_dump_mcb()
 */
char* mem_arena_dos_mcb(mem_arena_t* arena);

/**
 * @brief Gets remaining available bytes
 * @param arena Valid arena handle
 * @return Capacity - used bytes
 */
mem_size_t mem_arena_size(mem_arena_t* arena);

/**
 * @brief Gets total arena capacity
 * @param arena Valid arena handle
 * @return Maximum bytes allocatable
 */
mem_size_t mem_arena_capacity(mem_arena_t* arena);

/**
 * @brief Gets bytes currently allocated
 * @param arena Valid arena handle
 * @return Sum of all active allocations
 */
mem_size_t mem_arena_used(mem_arena_t* arena);

/**
 * @brief Gets memory allocation policy
 * @param arena Valid arena handle
 * @return policy eg MEM_ARENA_POLICY_DOS
 */
uint8_t mem_arena_policy(mem_arena_t* arena);

/**
 * @brief Gets pointer to the start of the arena
 * @param arena Valid arena handle
 * @return void* to arena->start
 */
void* mem_arena_base_address(mem_arena_t* arena);

/**
 * @brief Gets pointer to the start of the free section
 * @param arena Valid arena handle
 * @return void* to arena->start
 */
void* mem_arena_free_address(mem_arena_t* arena);

/* ----------------- Allocation ----------------- */

/**
 * @brief Allocates memory from arena
 * @param arena Valid arena handle
 * @param byte_request Size needed (will be aligned)
 * @return Pointer to memory or NULL if full
 *
 * @details Allocation Characteristics:
 *          - O(1) time complexity
 *          - No per-allocation overhead
 *          - Naturally aligned (8/16/32-bit)
 *
 * @warning Lifetime matches arena - no individual freeing
 */
void* mem_arena_alloc(mem_arena_t* arena, mem_size_t byte_request);

/**
 * @brief Allocates and zero-initializes memory from arena
 * @param arena Valid arena handle
 * @param byte_request Size needed (will be aligned)
 * @return Pointer to zeroed memory or NULL if full
 *
 * @note More efficient than separate alloc+memset for large blocks
 */
void* mem_arena_calloc(mem_arena_t* arena, mem_size_t byte_request);

/**
 * @brief Deallocates memory (no-op in current impl)
 * @param arena Valid arena handle
 * @param byte_request Size to theoretically free
 * @return Always returns NULL
 *
 * @deprecated Arena uses linear allocation only
 * @note Included for future expansion
 */
void* mem_arena_dealloc(mem_arena_t* arena, mem_size_t byte_request);

/* ----------------- Debugging ----------------- */

/**
 * @brief Dumps arena metadata to stream
 * @param output_stream File/console output
 * @param arena Valid arena handle
 *
 * @output Example:
 * @code
 * [DOS Arena @0x1234]
 * Policy: MEM_POLICY_DOS
 * Capacity: 64.0 KB
 * Used: 12.3 KB (19%)
 * MCB: 0x5678 (Owner: 0x0008)
 * @endcode
 */
void mem_arena_dump(FILE* output_stream, mem_arena_t* arena);

#endif
/** @} */ // end of memory_arena group
