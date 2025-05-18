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

bool file_pointer_is_eof(const dos_file_handle_t fhandle);

/**
* @retval old file pointer value
*/
dos_file_position_t file_pointer_reset(const dos_file_handle_t fhandle) {
    return dos_move_file_pointer(fhandle, 0, FSEEK_SET);
}

inline dos_file_position_t file_pointer_move(dos_file_handle_t fhandle, dos_file_position_t foffset, uint8_t forigin) {
    return dos_move_file_pointer(fhandle, foffset, forigin);
}

#endif
