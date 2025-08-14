#include "poweroff.h"
#include "../../terminal.h"
#include "../../../kernel/process.h"
#include "../../../drivers/vga.h"

int shell_poweroff_command(int argc, const char** argv) {
    term_write("Goodnight!", TC_WHITE);

    // No ACPI yet, so just halt

    asm volatile("cli");
    for (;;) {
        asm volatile("hlt");
    }

    return 0;
}