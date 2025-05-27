#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mem_arena.h"

#include "../DOS/dos_services.h"
#include "mem_constants.h"

typedef struct private_mem_arena {

  uint8_t		policy;		// e.g. MEM_POLICY_DOS or MEM_POLICY_C
  mem_address_t	start;		// base address of the arena
  char*			free;		// pointer to start of free memory within arena, initially free = start
  char*			end;		// end address limit of useable arena memory

} mem_arena_t;

static const mem_arena_t default_dos_mem_arena_t = { MEM_ARENA_POLICY_DOS, NULL, NULL, NULL };

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
/* ----------------- Public Interface ----------------- */

mem_arena_t* mem_arena_new(mem_arena_policy_t policy, mem_size_t byte_request) {
	assert(byte_request);
    switch(policy) {
        case MEM_ARENA_POLICY_DOS:
            return private_mem_arena_dos_new(byte_request);
        case MEM_ARENA_POLICY_C:
        default:
            fprintf(stderr, "Unimplemented policy: %d\n", policy);
            return NULL;
    }
}

mem_size_t mem_arena_delete(mem_arena_t* arena) {
    assert(arena);   
    switch(arena->policy) {
        case MEM_ARENA_POLICY_DOS:
            return private_mem_arena_dos_delete(arena);
        case MEM_ARENA_POLICY_C:	// TODO
        default:
            fprintf(stderr, "Unimplemented policy: %d\n", arena->policy);
            return 0;
    }
}

/* ----------------- Accessors ----------------- */

char* mem_arena_dos_mcb(mem_arena_t* arena) {
	assert(arena && arena->policy == MEM_ARENA_POLICY_DOS);
	mem_address_t m = arena->start;
	m.segoff.segment --;
	return m.ptr;
}

mem_size_t mem_arena_size(mem_arena_t* arena) {
	return arena->end - arena->free;
}

mem_size_t mem_arena_capacity(mem_arena_t* arena) {
	return arena->end - arena->start.ptr;
}

mem_size_t mem_arena_used(mem_arena_t* arena) {
	return mem_arena_capacity(arena) - mem_arena_size(arena);
}

/* ----------------- Allocation ----------------- */

void* mem_arena_alloc(mem_arena_t* arena, mem_size_t byte_request) {
	assert(arena && byte_request);
	if (byte_request <= mem_arena_size(arena)) {
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

void* mem_arena_dealloc(mem_arena_t* arena, mem_size_t byte_request) {
	assert(arena && byte_request);
	if (byte_request <= mem_arena_used(arena)) {
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
