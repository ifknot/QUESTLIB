#ifndef TDD_ON_ESCAPE_H
#define TDD_ON_ESCAPE_H

#if defined(__WATCOMC__)
    #include <bios.h>
#else
    // Define stubs for Clang
    #define _bios_keybrd(x) (0)
#endif

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
        #define ON_ESCAPE(action) ON_ESCAPE_BIOS(action)
    #else
        #include <conio.h> // Watcom's conio.h (kbhit/getch)
        #define ON_ESCAPE(action) ON_ESCAPE_CONIO(action)
    #endif
#else
    #error "Escape key detection not implemented for this compiler!"
#endif

#endif
