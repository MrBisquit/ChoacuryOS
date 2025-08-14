#include "exit.h"

#include "../../terminal.h"
#include "../../../drivers/vga.h"
#include "../../../kernel/process.h"

int shell_exit_command(int argc, const char** argv) {
    term_write("Goodbye!\n", TC_WHITE);
    terminate_process();
}