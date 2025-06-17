#ifndef TDD_ON_ESCAPE_H
#define TDD_ON_ESCAPE_H

#include <stdio.h>

#if defined(__WATCOMC__)
    #include <bios.h>
#else
    // Define stubs for Clang
    #define _bios_keybrd(x) (0)
#endif

#define PRESS_ENTER_BIOS \
    do { \
        printf("\nPress Enter to continue..."); \
        while (_bios_keybrd(_KEYBRD_READ) != 0x1C0D); /* Enter scan code */ \
    } while(0)

#define PRESS_ENTER_C \
    do { \
        printf("\nPress Enter to continue..."); \
        fflush(stdout); \
        while (getchar() != '\n'); /* Wait for Enter */ \
    } while(0)

// Macro: Checks for ESC in non-blocking way (BIOS scan code 0x011B)
// and performs action eg ON_ESCAPE(break;)
#define ON_ESCAPE_BIOS(action) \
    if (_bios_keybrd(_KEYBRD_READY)) { \
        if (_bios_keybrd(_KEYBRD_READ) == 0x011B) { \
            action \
        } \
    }

// Macro: Checks for ESC (ASCII 27)
#define ON_ESCAPE_CONIO(action) \
    if (kbhit()) { \
        if (getch() == 27) { \
            action \
        } \
    }

#if defined(__WATCOMC__)
    #if defined(_BIOS_H)
        #include <bios.h>
        #define ON_ESCAPE(action) ON_ESCAPE_
        BIOS(action)
        #define PRESS_ENTER PRESS_ENTER_BIOS;
    #else
        #include <conio.h> // Watcom's conio.h (kbhit/getch)
        #define ON_ESCAPE(action) ON_ESCAPE_CONIO(action)
        #define PRESS_ENTER PRESS_ENTER_C;
    #endif
#else
    #error "Escape key detection not implemented for this compiler!"
#endif

#endif
