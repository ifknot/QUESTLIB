/**
 * @file mem_arena.c
 * @brief Linear memory allocator implementation
 * @defgroup memory_arena_impl Memory Arena Internals
 * @{
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "mem_arena.h"

#include "../DOS/dos_services.h"
#include "mem_constants.h"
#include "mem_tools.h"
#include "mem_types.h"

/* ----------------- Arena Structure ----------------- */

/**
 * @brief Internal arena representation
 * @dot
 * digraph arena_struct {
 *     node [shape=record, fontname="Courier New"];
 *     arena [label="{
 *         <policy> policy|
 *         <start> start (base address)|
 *         <free> free (current position)|
 *         <end> end (boundary)
 *     }"];
 * }
 * @enddot
 */
typedef struct private_mem_arena_t {
    uint8_t policy;         ///< MEM_ARENA_POLICY_DOS or MEM_ARENA_POLICY_C
    mem_address_t start;    ///< Base address of allocated memory
    char* free;             ///< Current allocation pointer
    char* end;              ///< End of available memory
} mem_arena_t;

/// Default-initialized DOS arena template
static const mem_arena_t default_dos_mem_arena_t = {
    MEM_ARENA_POLICY_DOS,
    {NULL}, NULL, NULL
};

/* ----------------- DOS-Specific Implementation ----------------- */

/**
 * @brief Creates a DOS memory arena via INT 21h
 * @param byte_count Requested size in bytes
 * @return Initialized arena or NULL on failure
 *
 * @details Memory Allocation:
 * @code
 * Paragraphs = (bytes / 16) + (bytes % 16 ? 1 : 0)
 * INT 21h, AH=48h:
 *   BX = Paragraphs requested
 *   Returns:
 *     AX = Segment address (success)
 *     BX = Max available (failure)
 * @endcode
 *
 * @warning Maximum allocatable:
 *          - 65535 paragraphs (1MB - 16 bytes)
 *          - Typically limited to 640KB in practice
 */
mem_arena_t* private_mem_arena_dos_new(mem_size_t byte_count) {
	assert(byte_count);
	if(!byte_count) {
	    return NULL;
	}
	mem_arena_t* arena = (mem_arena_t*)malloc(sizeof(mem_arena_t));
    assert(arena != NULL);
    *arena = default_dos_mem_arena_t;
    mem_size_t paragraphs = (byte_count / MEM_SIZE_PARAGRAPH) + ((byte_count % MEM_SIZE_PARAGRAPH) ? 1 : 0);
    arena->start.segoff.segment = dos_allocate_memory_blocks(paragraphs);
    if (arena->start.segoff.segment) {
        arena->free = arena->start.ptr;
        arena->end = arena->start.ptr + (paragraphs * MEM_SIZE_PARAGRAPH);
    }
#ifndef NDEBUG
    else {
        fprintf(stderr, "DOS allocation failed: Requested %lu bytes (%u paragraphs)\n", byte_count, paragraphs);
    }
#endif
    return arena;
}

/**
 * @brief Releases DOS memory arena
 * @param arena Valid DOS arena
 * @return Bytes freed
 *
 * @details Uses INT 21h, AH=49h:
 *          - ES = Segment to free
 *          - All allocations become invalid
 */
mem_size_t private_mem_arena_dos_delete(mem_arena_t* arena) {
	assert(arena);
    mem_size_t freed = mem_arena_capacity(arena);
    dos_free_allocated_memory_blocks(arena->start.segoff.segment);
    free(arena);
    return freed;
}

/* ----------------- C99-Specific Implementation ----------------- */

/**
 * @brief Creates a C memory arena via malloc
 * @param byte_count Requested size in bytes
 * @return Initialized arena or NULL on failure
 */
mem_arena_t* private_mem_arena_c_new(mem_size_t byte_count) {
    assert(byte_count > 0);
    if (!byte_count) {
        return NULL;
    }

    mem_arena_t* arena = (mem_arena_t*)malloc(sizeof(mem_arena_t));
    if (!arena) {
        return NULL;
    }

    arena->policy = MEM_ARENA_POLICY_C;
    arena->start.ptr = malloc(byte_count);
    if (!arena->start.ptr) {
        free(arena);
        return NULL;
    }

    arena->free = arena->start.ptr;
    arena->end = arena->start.ptr + byte_count;

    return arena;
}

/**
 * @brief Releases C memory arena
 * @param arena Valid C arena
 * @return Bytes freed
 */
mem_size_t private_mem_arena_c_delete(mem_arena_t* arena) {
    assert(arena && arena->policy == MEM_ARENA_POLICY_C);
    mem_size_t freed = mem_arena_capacity(arena);
    free(arena->start.ptr);
    free(arena);
    return freed;
}


/* ----------------- Public Interface ----------------- */

mem_arena_t* mem_arena_create(mem_arena_policy_t policy, mem_size_t byte_request) {
	assert(byte_request);
    switch(policy) {
        case MEM_ARENA_POLICY_DOS:
            return private_mem_arena_dos_new(byte_request);
        case MEM_ARENA_POLICY_C:
            return private_mem_arena_c_new(byte_request);
        default:
            fprintf(stderr, "Unimplemented policy: %d\n", policy);
            return NULL;
    }
}

mem_size_t mem_arena_delete(mem_arena_t* arena) {
    assert(arena);
    if(!arena) {
        return 0;
    }
    switch(arena->policy) {
        case MEM_ARENA_POLICY_DOS:
            return private_mem_arena_dos_delete(arena);
        case MEM_ARENA_POLICY_C:
            return private_mem_arena_c_delete(arena);
        default:
            fprintf(stderr, "Unimplemented policy: %d\n", arena->policy);
            return 0;
    }
}

/* ----------------- Accessors ----------------- */

char* mem_arena_dos_mcb(mem_arena_t* arena) {
	assert(arena && arena->policy == MEM_ARENA_POLICY_DOS);
	if(!arena || arena->policy != MEM_ARENA_POLICY_DOS) {
	    return NULL;
	}
	mem_address_t m = arena->start;
	--m.segoff.segment;
	return m.ptr;
}

mem_size_t mem_arena_size(mem_arena_t* arena) {
	return mem_diff_pointers(arena->end, arena->free);
}

mem_size_t mem_arena_capacity(mem_arena_t* arena) {
	return mem_diff_pointers(arena->end, arena->start.ptr);
}

mem_size_t mem_arena_used(mem_arena_t* arena) {
	return mem_arena_capacity(arena) - mem_arena_size(arena);
}

uint8_t mem_arena_policy(mem_arena_t* arena) {
    return arena->policy;
}

void* mem_arena_base_address(mem_arena_t* arena) {
    return arena->start.ptr;
}

void* mem_arena_free_address(mem_arena_t* arena) {
    return arena->free;
}

/* ----------------- Allocation ----------------- */

void* mem_arena_alloc(mem_arena_t* arena, mem_size_t byte_request) {
	if (arena && byte_request && byte_request <= mem_arena_size(arena)) {
        void* ptr = arena->free;
        arena->free += byte_request;
        return ptr;
    }
#ifndef NDEBUG
    fprintf(stderr, "Allocation failed: Requested %lu, Available %lu\n",
           byte_request, mem_arena_size(arena));
#endif
    return NULL;
}

void* mem_arena_calloc(mem_arena_t* arena, mem_size_t byte_request) {
    void* ptr = mem_arena_alloc(arena, byte_request);
    if (ptr) {
        #if defined(__WATCOMC__) && defined(__386__) // Use optimized platform-specific zeroing
        _fmemset(ptr, 0, byte_request);  // Watcom fast memset
        #else
        memset(ptr, 0, byte_request);
        #endif
    }
    return ptr;
}

void* mem_arena_dealloc(mem_arena_t* arena, mem_size_t byte_request) {
	if (arena && byte_request && byte_request <= mem_arena_used(arena)) {
        arena->free -= byte_request;
        return arena->free;
    }
#ifndef NDEBUG
    fprintf(stderr, "Deallocation failed: Requested %lu, Used %lu\n", byte_request, mem_arena_used(arena));
#endif
    return NULL;
}

/* ----------------- Debugging ----------------- */

void mem_arena_dump(FILE* output_stream, mem_arena_t* arena) {
    if (!output_stream || !arena) return;

    fprintf(output_stream,
           "\nArena @%p\n"
           "Policy: %s\n"
           "Range: [%p - %p]\n"
           "Capacity: %lu bytes\n"
           "Used: %lu bytes\n"
           "Free: %lu bytes\n",
           arena,
           mem_policy_info[arena->policy],
           arena->start.ptr,
           arena->end,
           mem_arena_capacity(arena),
           mem_arena_used(arena),
           mem_arena_size(arena));

    if (arena->policy == MEM_ARENA_POLICY_DOS) {
        fprintf(output_stream, "MCB: %p\n", mem_arena_dos_mcb(arena));
    }

    fflush(output_stream);
}

/** @} */ // end of memory_arena_impl group
