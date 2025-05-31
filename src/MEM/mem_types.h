/**
 * @file mem_types.h
 * @brief DOS memory addressing types and utilities
 * @defgroup memory_types Memory Types
 * @{
 */

#ifndef MEM_TYPES_H
#define MEM_TYPES_H

#include <stddef.h>
#include <stdint.h>

/* ----------------- Size Types ----------------- */

/**
 * @brief Unsigned memory size type (32-bit)
 * @details Used for all memory size measurements
 */
typedef uint32_t mem_size_t;

/**
 * @brief Signed memory difference type (32-bit)
 * @details Used for pointer arithmetic and offsets
 */
typedef int32_t mem_diff_t;

/* ----------------- Segment:Offset Addressing ----------------- */

/**
 * @brief Intel 16-bit segment:offset memory address
 * @dot
 * digraph segoff {
 *     node [shape=record, fontname="Courier New"];
 *     segoff [label="<segment> Segment (16-bit)|<offset> Offset (16-bit)"];
 * }
 * @enddot
 *
 * @note Little-endian layout matches Intel architecture
 * @warning Physical address = (segment << 4) + offset
 */
typedef struct {
    uint16_t offset;  ///< Low 16 bits (0x0000-0xFFFF)
    uint16_t segment; ///< High 16 bits (0x0000-0xFFFF)
} mem_segoff_t;

/// Default zero-initialized segment:offset pair
static const mem_segoff_t default_mem_segoff_t = {0, 0};

/* ----------------- Address Union ----------------- */

/**
 * @brief Flexible 32-bit memory address representation
 * @dot
 * digraph address_union {
 *     node [shape=record, fontname="Courier New"];
 *     union [label="<ptr> char* ptr|<memloc> uint32_t linear|<segoff> mem_segoff_t segmented|<words> uint16_t words[2]|<bytes> uint8_t bytes[4]"];
 * }
 * @enddot
 *
 * @details Allows multiple views of the same 32-bit address:
 * @code
 * mem_address_t addr;
 * addr.memloc = 0x12345678;
 * printf("Segment: %04X, Offset: %04X\n",
 *        addr.segoff.segment, addr.segoff.offset);
 * @endcode
 *
 * @note Useful for:
 *       - DOS real-mode/protected mode transitions
 *       - Pointer arithmetic
 *       - Memory debugging
 */
typedef union {
    char* ptr;           ///< As native pointer
    uint32_t memloc;     ///< Flat 32-bit address
    mem_segoff_t segoff; ///< Segment:offset pair
    uint16_t words[2];   ///< As two 16-bit words
    uint8_t bytes[4];    ///< As four 8-bit bytes
} mem_address_t;

#endif
/** @} */ // end of memory_types group
