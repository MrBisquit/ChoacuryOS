#include "command.h"

// Command includes
#include "guiload/guiload.h"

// Temp
int example_command(int argc, const char** argv) {

}

int temp_shell_guiload_command(int argc, char** argv) {
    return 0;
}

void shell_init_commands_list() {
    /*static Command shell_commands_list[] = {
        //{"example", "arg1 arg2", "Example command", example_command},
        {"guiload", "No args.", "Loads up the GUI (WIP!)", shell_guiload_command}
    };*/
}

Command shell_commands_list[] = {
    //{"example", "arg1 arg2", "Example command", example_command},
    {"guiload", "No args.", "Loads up the GUI (WIP!)", shell_guiload_command}
};