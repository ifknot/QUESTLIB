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
	if (!last_dot || last_dot == file_path) {
		return NULL;
	}
	else {
		return ++last_dot;
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

size_t read_large_file(const dos_file_handle_t fhandle, void* buffer, uint32_t offset, size_t bytes_to_read) {
    uint8_t* current_buffer = (uint8_t*)buffer;
    size_t remaining_bytes = bytes_to_read;
    size_t total_read = 0;
    if (dos_move_file_pointer(fhandle, offset, SEEK_SET) == -1) { // Set initial file position
        return -1;
    }
    while (remaining_bytes > 0) {
        const uint16_t chunk_size = (remaining_bytes > 0xFF00) ? 0xFF00 : (uint16_t)remaining_bytes; //DOS read limit per call: 0xFF00 bytes (65280)
        const int16_t bytes_read = dos_read_file(fhandle, current_buffer, chunk_size);
        if (bytes_read <= 0) { Handle read errors or EOF
            break;
        }
        total_read += bytes_read;
        current_buffer += bytes_read;
        remaining_bytes -= bytes_read;
        if (bytes_read < chunk_size) { //Check for partial read
            break;
        }
    }
    return total_read;
}
