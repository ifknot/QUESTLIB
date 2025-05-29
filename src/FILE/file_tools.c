#include "file_tools.h"

#include <string.h>

#include "file_constants.h"

#include "../DOS/dos_services_files.h"
#include "../DOS/dos_services_files_types.h"

const char* file_get_extension(const char* file_path) {
	if (!file_path) {
		return NULL;
	}
	const char* last_dot = strrchr(file_path, FILE_EXTENSION_DELIM);
	if (!last_dot || p == file_path) {
		return NULL;
	}
	else {
		return ++p;
	}
}

file_size_t file_get_size(const dos_file_handle_t fhandle) {
    const dos_file_position_t original_pos = dos_move_file_pointer(fhandle, 0, FSEEK_CUR);
    const dos_file_position_t size = dos_move_file_pointer(fhandle, 0, FSEEK_END);
    dos_move_file_pointer(fhandle, original_pos, FSEEK_SET); // Restore position using original call's return value
    return size;
}

bool file_position_indicator_is_eof(const dos_file_handle_t fhandle) {
    const dos_file_position_t current_pos = dos_move_file_pointer(fhandle, 0, FSEEK_CUR);
    const dos_file_position_t end_pos = dos_move_file_pointer(fhandle, 0, FSEEK_END);
    dos_move_file_pointer(fhandle, current_pos, FSEEK_SET);
    return current_pos == end_pos;
}
