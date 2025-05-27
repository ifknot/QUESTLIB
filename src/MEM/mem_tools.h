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

/* ----------------- Memory Analysis ----------------- */

/**
 * @brief Queries available DOS memory
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
uint16_t mem_available_low_paragraphs();

 /**
 * Offset Size		Description
 *	00   byte		'M' 4Dh  member of a MCB chain, (not last)
 *					'Z' 5Ah  indicates last entry in MCB chain
 *					other values cause "Memory Allocation Failure" on exit
 *	01   word		PSP segment address of MCB owner (Process Id)
 *					possible values:
 *						0 = free
 *						8 = Allocated by DOS before first user pgm loaded
 *						other = Process Id/PSP segment address of owner
 *	03   word		number of paras related to this MCB (excluding MCB)
 *	05	3bytes		reserved
 *	08  8bytes		ASCII program name, NULL terminated if less than max length (DOS 4.x+)
 *	10  nbytes	first byte of actual allocated memory block
 *
 * @note  An undocumented way to find the first or 'base' MemBlockRec to use DOS Fn 52H.
 */
void mem_dump_mcb(char* mcb);

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
