/**
 * @defgroup dos_services DOS Interrupt Services
 * @brief Collection of functions for DOS interrupt services
 * @details Provides safe wrappers around fundamental DOS interrupt services
 * for memory management and interrupt vector manipulation.
 * @{
 */
#include "dos_services.h"

#include <stdio.h>

#include "dos_services_constants.h"
#include "dos_services_types.h"
#include "dos_error_messages.h"

/**
 * @brief Provides a safe method for changing interrupt vectors
 * @details Uses INT 21h, AH=25h to set an interrupt vector.
 * @note This operation is critical and should be used carefully as it modifies system interrupt table.
 *
 * @param vec_num The interrupt vector number to modify (0-255)
 * @param phandler Pointer to the new interrupt handler function
 *
 * @asm
 *   INT 21,25 - Set Interrupt Vector
 *   AH = 25h
 *   AL = interrupt number
 *   DS:DX = pointer to interrupt handler
 * @endasm
 */
void dos_set_interrupt_vector(uint8_t vec_num, void* phandler) {
    __asm {
        .8086
        pushf                                ; preserve what int 21h may not
        push    ds                           ; due to unreliable behaviour

        lds     dx, phandler                ; copy pointer to handler into DS:DX
        mov     al, vec_num                 ; interrupt vector number
        mov     ah, DOS_SET_INTERRUPT_VECTOR    ; 25h service
        int     DOS_SERVICE

        pop     ds
        popf
    }
}

/**
 * @brief Retrieves the current interrupt vector
 * @details Uses INT 21h, AH=35h to get an interrupt vector address.
 *
 * @param vec_num The interrupt vector number to query (0-255)
 * @return void* Segment:offset pointer to current interrupt handler (little-endian format)
 *
 * @asm
 *   INT 21,35 - Get Interrupt Vector
 *   AH = 35h
 *   AL = interrupt vector number
 *   Returns:
 *   ES:BX = pointer to interrupt handler
 * @endasm
 */
void* dos_get_interrupt_vector(uint8_t vec_num) {
    void* phandler = 0;
    __asm {
        .8086
        pushf
        push    ds

        mov     al, vec_num                 ; interrupt vector number
        mov     ah, DOS_GET_INTERRUPT_VECTOR    ; 35h service
        int     DOS_SERVICE
        lea     di, phandler
        mov     [di], bx                    ; copy segment into address_t (little endian)
        mov     [di + 2] , es               ; copy offset

        pop     ds
        popf
    }
    return phandler;
}

/**
 * @brief Allocates memory blocks in paragraphs
 * @details Uses INT 21h, AH=48h to allocate memory in 16-byte paragraphs.
 * @warning Each allocation requires a 16-byte overhead for the Memory Control Block (MCB).
 *
 * @param paragraphs Number of 16-byte paragraphs requested
 * @return uint16_t Segment address of allocated block (AX:0000) or 0 if failed
 *
 * @asm
 *   INT 21,48 - Allocate Memory
 *   AH = 48h
 *   BX = number of memory paragraphs requested
 *   Returns:
 *   AX = segment address of allocated block (MCB + 1 para) if successful
 *      = error code if CF set
 *   BX = size of largest available block if AX=8 (insufficient memory)
 *   CF = 0 if success, 1 if error
 * @endasm
 *
 * @retval 0 Allocation failed
 * @retval >0 Segment address of allocated memory
 *
 * @note To find available memory size, set BX=FFFFh before calling (will return error but BX contains available memory)
 * @see dos_free_allocated_memory_blocks()
 */
uint16_t dos_allocate_memory_blocks(uint16_t paragraphs) {
    if(!paragraphs) {
        return paragraphs;
    }
    uint16_t available, mem_seg;
    dos_error_code_t err_code = 0;
    __asm {
    .8086
    pushf
    push    ds

    mov     bx, paragraphs              ; number requested paragraphs
    mov     ah, DOS_ALLOCATE_MEMORY_BLOCKS  ; allocate memory
    int     DOS_SERVICE                 ; 48h service
    jnc     OK                          ; success CF = 0
    mov     err_code, ax                ; CF set, and AX = 08 (Not Enough Mem)
    mov     available, bx               ; size in paras of the largest block of memory available
    mov     ax, 0
OK: mov     mem_seg, ax

    pop     ds
    popf
    }
#ifndef NDEBUG
    if (err_code) {
        fprintf(stderr, "%s", dos_error_messages[err_code]);
        if (err_code == DOS_INSUFFICIENT_MEMORY) {
            fprintf(stderr, " largest available block = %u paragraphs\n", available);    // paragraph = 16 bytes
        }
    }
#endif
    return mem_seg;
}

/**
 * @brief Frees previously allocated memory blocks
 * @details Uses INT 21h, AH=49h to release memory allocated by dos_allocate_memory_blocks().
 * @warning Unpredictable results if memory wasn't allocated by current process or wasn't allocated properly.
 *
 * @param segment Segment address of memory to free (MCB + 1 paragraph)
 * @return uint16_t DOS error code (0 if success)
 *
 * @asm
 *   INT 21,49 - Free Allocated Memory
 *   AH = 49h
 *   ES = segment of block to free (MCB + 1 para)
 *   Returns:
 *   AX = error code if CF set
 *   CF = 0 if success, 1 if error
 * @endasm
 *
 * @retval 0 Success
 * @retval 7 Memory control blocks destroyed
 * @retval 8 Insufficient memory (shouldn't occur for free operation)
 *
 * @note Unreliable in TSR programs once resident, as COMMAND.COM and others take all memory when loading.
 * @see dos_allocate_memory_blocks()
 */
uint16_t dos_free_allocated_memory_blocks(uint16_t segment) {
    dos_error_code_t err_code = 0;
    char* mcb = (char*)(((int32_t)segment - 1) << 16);
    __asm {
        .8086
        pushf
        push    ds

        mov     ax, segment                         ; the segment to be released
        mov     es, ax                              ; segment of the block to be returned(MCB + 1para)
        mov     ah, DOS_FREE_ALLOCATED_MEMORY_BLOCKS    ; de-allocate memory
        int     DOS_SERVICE                         ; dos call 49h
        jnc     OK                                  ; success CF = 0
        mov     err_code, ax                        ; de-allocation failed ax is dos error code
    OK:
        pop     ds
        popf
    }
    mcb[0] = '\0';  // invalidate the MCB
#ifndef NDEBUG
    if (err_code) {
        fprintf(stderr, "%s %X\n", dos_error_messages[err_code], segment);
    }
#endif
    return err_code;
}

/** @} */ // end of dos_services group
