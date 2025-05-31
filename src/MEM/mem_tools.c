#include "mem_tools.h"

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../DOS/dos_services_files.h"

uint16_t mem_max_paragraphs() {
    uint16_t paragraphs, err_code;
    paragraphs = err_code = 0;
    __asm {
        .8086

        mov     bx, 0FFFFh              ; deliberate over sized request
        mov     ah, 48h                 ; allocate memory
        int     21h                     ; INT 21h, 48h allocate memory service
        mov     err_code, ax            ; CF set, and AX = 08 (Not Enough Mem)
        mov     paragraphs, bx          ; size in paras of the largest block of memory available

    }
    assert(err_code == 8);
    return paragraphs;
}

mem_diff_t mem_diff_pointers(const void* p1, const void* p2) {
    const uintptr_t addr1 = (uintptr_t)p1;  // uintptr_t is more portable than uint32_t
    const uintptr_t addr2 = (uintptr_t)p2;

    /* The cast to ptrdiff_t ensures proper signed result */
    return (mem_diff_t)(addr1 - addr2);
}

void mem_dump_mcb_to_stream( FILE* stream, const char* mcb) {
    assert(mcb != NULL);
    assert(stream != NULL);

    fprintf(stream,
        "\nMCB - DOS Memory Control Block @%p\n"
        "Offset\tSize\tValue\n"
        "00h\tbyte\t%c (%s)\n"
        "01h\tword\t%04X (%s)\n"
        "03h\tword\t%u paragraphs (%u bytes)\n"
        "08h\t8bytes\t\"%.8s\"\n",
        mcb,
        mcb[0],
        (mcb[0] == 'M') ? "Middle block" :
        (mcb[0] == 'Z') ? "Last block" : "Invalid",
        *(const uint16_t*)(mcb + 1),
        (*(const uint16_t*)(mcb + 1) == 0) ? "Free" :
        (*(const uint16_t*)(mcb + 1) == 8) ? "DOS System" : "Program Owned",
        *(const uint16_t*)(mcb + 3),
        *(const uint16_t*)(mcb + 3) * 16,  // Convert paragraphs to bytes
        mcb + 8
    );
}


dos_file_size_t mem_load_from_file(const char* path_name, char* start, uint16_t nbytes) {
    assert(path_name && strlen(path_name) > 0 && start && nbytes);
    dos_file_handle_t fhandle = dos_open_file(path_name, ACCESS_READ_ONLY);
    dos_file_size_t bytes_loaded = 0;
    if (fhandle) {
        bytes_loaded = dos_read_file(fhandle, start, nbytes);
        dos_close_file(fhandle);
    }
    return bytes_loaded;
}

dos_file_size_t mem_save_to_file(const char* path_name, char* start, uint16_t nbytes){
    assert(path_name && strlen(path_name) > 0 && start && nbytes);
    dos_file_size_t bytes_saved = 0;
    dos_file_handle_t fhandle = dos_open_file(path_name, ACCESS_WRITE_ONLY);
    if (fhandle) {
        bytes_saved = dos_write_file(fhandle, start, nbytes);
        dos_close_file(fhandle);
    }
    return bytes_saved;
}
