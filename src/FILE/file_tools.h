#ifndef FILE_EXTENSION_H
#define	FILE_EXTENSION_H

#include <stdbool.h>

#include "file_types.h"
#include "../DOS/dos_services_files_types.h"
#include "../DOS/dos_services_files.h"

/**
*  @brief	Retrieve position of file extension (if there is one) in file path
*  @param	const char* file_path, ptr to extant file path C string
*  @retval  const char* position of start of extension in file_path
*/
const char* file_get_extension(const char* file_path);

file_size_t file_get_size(const dos_file_handle_t fhandle);

/**
* One of the attributes of an open file is its file position that keeps track of where in the file the next character is to be read or written. 
* On GNU systems, and all POSIX.1 systems, the file position is simply an integer representing the number of bytes from the beginning of the file.
*/
bool file_position_indicator_is_eof(const dos_file_handle_t fhandle);

/**
* @retval old file position indicator value
*/
dos_file_position_t file_position_indicator_reset(const dos_file_handle_t fhandle) {
    return dos_move_file_pointer(fhandle, 0, FSEEK_SET); // DOS int 21, 42 function is called "Move File Pointer Using Handle" in the documentation 
}

inline dos_file_position_t file_position_indicator_move(dos_file_handle_t fhandle, dos_file_position_t foffset, uint8_t forigin) {
    return dos_move_file_pointer(fhandle, foffset, forigin);
}

#endif
