/**
 * @file file_tools.c
 * @brief File path and handle utilities
 * @defgroup file_tools File Handling Extras
 * @{
 */
#ifndef FILE_EXTENSION_H
#define	FILE_EXTENSION_H

#include <stdbool.h>
#include <assert.h>

#include "file_types.h"
#include "../DOS/dos_services_files_types.h"
#include "../DOS/dos_services_files.h"

/**
 * @brief Extracts file extension from path
 * @param file_path Null-terminated path string
 * @return Pointer to extension or NULL if none exists
 * 
 * @retval NULL for:
 *         - NULL input
 *         - No extension found
 *         - Hidden files (".profile")
 *         - Trailing dots ("file.")
 */
const char* file_get_extension(const char* file_path);

/**
 * @brief Gets file size without modifying position
 * @param fhandle Valid DOS file handle
 * @return File size in bytes or -1 on error
 * 
 * @details Uses seek-to-end method
 * @warning Preserves original file position
 */
file_size_t file_get_size(const dos_file_handle_t fhandle);

/**
 * @brief Checks if file pointer is at EOF
 * @param fhandle Valid DOS file handle
 * @return True if at end of file, false otherwise
 * 
 * @note More reliable than feof() for DOS handles
 * @warning Temporarily modifies file position
 */
bool file_position_indicator_is_eof(const dos_file_handle_t fhandle);

/**
 * @brief Resets file position indicator to start of file
 * @param fhandle Valid DOS file handle
 * @return Previous file position or -1 on error
 * 
 * @retval >=0 Previous file position
 * @retval -1 Error (invalid handle or I/O error)
 * 
 * @note Wrapper for DOS "Move File Pointer Using Handle" function
 */
inline dos_file_position_t file_position_indicator_reset(const dos_file_handle_t const fhandle) {
    return dos_move_file_pointer(fhandle, 0, FSEEK_SET); // DOS int 21, 42 function is called "Move File Pointer Using Handle" in the documentation 
}

/**
 * @brief Moves file position indicator
 * @param fhandle Valid DOS file handle
 * @param offset Byte offset to move
 * @param origin Starting position (SEEK_SET/SEEK_CUR/SEEK_END)
 * @return Previous file position or -1 on error
 * 
 * @details Uses DOS INT 21h AH=42h:
 *          - SEEK_SET (0) = Beginning of file
 *          - SEEK_CUR (1) = Current position
 *          - SEEK_END (2) = End of file
 * 
 * @retval >=0 Previous file position
 * @retval -1 Error (invalid handle/offset)
 * 
 * @warning Large offsets (>32MB) may require DOS 3.3+
 */
inline dos_file_position_t file_position_indicator_move(const dos_file_handle_t fhandle, const dos_file_position_t foffset, const uint8_t forigin) {
    assert(origin == SEEK_SET || origin == SEEK_CUR || origin == SEEK_END);
    return dos_move_file_pointer(fhandle, foffset, forigin);
}

#endif

/** @} */ // end of file_tools group
