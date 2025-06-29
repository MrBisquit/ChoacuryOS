#include "panic.h"
#include "../drivers/vga.h"
#include "../shell/terminal.h"
#include "../drivers/vbe.h"

__attribute__((noreturn))
void panic_impl(const char* location_prefix, const char* message)
{
    term_write(location_prefix, TC_LRED);
    term_putchar(' ', TC_LRED);
    term_write(message, TC_LRED);

    
    // Attempt to draw VBE too
    // Experimental, remove if it causes issues
    //vbe_clear_screen(0x000000ff); // Blue
    //print_string(location_prefix, 0, 0, 0x00ffffff); // White
    //print_string(message, 0, 20, 0x00ffffff); // White

    asm volatile("cli");
    for (;;) {
        asm volatile("hlt");
    }
    __builtin_unreachable();
}
