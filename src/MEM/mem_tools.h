/**
 * @file mem_tools.h
 * @brief DOS memory management utilities
 * @defgroup memory_tools Memory Tools
 * @{
 */
#ifndef MEM_TOOLS_H
#define MEM_TOOLS_H

#include <stdint.h>
#include <stdio.h>

#include "../DOS/dos_services_files_types.h"

#include "mem_types.h"

/* ----------------- Memory Analysis ----------------- */

/**
 * @brief Queries available DOS memory (ie "low memory")
 * @return Available memory in paragraphs (uint16_t)
 *
 * @details Uses DOS INT 21h, Function 48h:
 *          - Sets BX=FFFFh to probe maximum available memory
 *          - Returns actual available paragraphs in BX
 *          - Error can be safely ignored (DOS 640K limit)
 *
 * @note 1 paragraph = 16 bytes
 * @see mem_dump_mcb()
 */
uint16_t mem_max_paragraphs();

/**
 * @brief Calculates the byte difference between two memory addresses
 * @ingroup memory_management
 *
 * @param p1 First memory address (can be near/far/huge pointer)
 * @param p2 Second memory address (can be near/far/huge pointer)
 * @return ptrdiff_t Signed difference in bytes (p1 - p2)
 *
 * @note For 16-bit segmented architectures (DOS):
 *       - Near pointers: Returns offset difference only
 *       - Far pointers: Returns 32-bit linear address difference
 *       - Huge pointers: Returns normalized difference
 *
 * @warning Pointer arithmetic limitations:
 *          - Result may overflow if pointers are too far apart
 *          - For far pointers, difference assumes same segment
 *          - Not suitable for comparing unrelated memory regions
 *
 * @example
 * int arr[10];
 * ptrdiff_t diff = mem_difference_pointers(&arr[5], &arr[2]);
 * // diff == 3 * sizeof(int)
 */
mem_diff_t mem_diff_pointers(const void* p1, const void* p2);

/**
 * @brief Dumps Memory Control Block (MCB) information to a specified stream
 * @param[in] mcb Pointer to the Memory Control Block
 * @param[in] stream Output stream (e.g., stdout, stderr, file)
 *
 * @details Prints formatted MCB information including:
 *          - Chain marker ('M' or 'Z')
 *          - Owner PSP segment
 *          - Paragraph count
 *          - Program name (if available)
 *
 * @pre mcb != NULL (asserted)
 * @pre stream != NULL (asserted)
 *
* @details DOS Version Compatibility:
 * @code
 * | Feature        | DOS 2.0 | DOS 3.0 | DOS 4.0 | DOS 5.0+ |
 * |----------------|---------|---------|---------|----------|
 * | MCB Chain      |   Yes   |   Yes   |   Yes   |   Yes    |
 * | PSP Ownership  |   Yes   |   Yes   |   Yes   |   Yes    |
 * | Program Name   |   No    |   No    |   Yes   |   Yes    |
 * | INT 21h/52h    |   No    |   Yes   |   Yes   |   Yes    |
 * @endcode
 *
 * @warning MCB structure is DOS-specific and undocumented
 * @see mem_available_low_paragraphs()
 */
void mem_dump_mcb_to_stream(FILE* stream, const char* mcb);

/* ----------------- File Operations ----------------- */

/**
 * @brief Loads raw data from file to memory
 * @param[in] path_name File to load (must be non-empty)
 * @param[out] start Destination memory address
 * @param[in] nbytes Maximum bytes to load (≤64KB, must be >0)
 * @return Actual bytes loaded (dos_file_size_t)
 *
 * @details Features:
 *          - Handles up to one 64K page
 *          - Preserves raw byte values
 *          - No format conversion
 *
 * @pre path_name != NULL && strlen(path_name) > 0 (asserted)
 * @pre start != NULL (asserted)
 * @pre nbytes > 0 (asserted)
 * @warning No bounds checking on destination
 * @see mem_save_to_file()
 */
dos_file_size_t mem_load_from_file(const char* path_name, char* start, uint16_t nbytes);

/**
 * @brief Saves raw memory to file
 * @param[in] path_name Destination file (must be non-empty)
 * @param[in] start Source memory address
 * @param[in] nbytes Bytes to save (≤64KB, must be >0)
 * @return Actual bytes saved (dos_file_size_t)
 *
 * @details Features:
 *          - Handles up to one 64K page
 *          - Writes unmodified memory contents
 *          - Creates/overwrites files
 *
 * @pre path_name != NULL && strlen(path_name) > 0 (asserted)
 * @pre start != NULL (asserted)
 * @pre nbytes > 0 (asserted)
 * @see mem_load_from_file()
 */
dos_file_size_t mem_save_to_file(const char* path_name, char* start, uint16_t nbytes);


#endif

/** @} */ // end of memory_tools group
