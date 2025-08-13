/*
    The kernel of Choacury
    I call it the Choakern! The code still needs to be finished tho...
*/

/* Includes needed for the kernel to actually work.*/
#include "../drivers/gdt.h"
#include "../drivers/idt.h"
#include "../drivers/keymaps/ps2_keymap_us.h"   // <-- US Keyboard Layout.
#include "../drivers/mouse.h"
#include "../drivers/pci.h"
#include "../drivers/pic.h"
#include "../drivers/pit.h"
#include "../drivers/ports.h"
#include "../drivers/ps2.h"
#include "../drivers/sound.h"
#include "../drivers/storage/device.h"
#include "../drivers/types.h"
#include "../drivers/utils.h"
#include "../drivers/vga.h"
//#include "../drivers/fat.h"
#include "../memory/kmalloc.h"
#include "../memory/pmm.h"
#include "../shell/shell.h"
#include "../shell/terminal.h"
#include "multiboot.h"
#include "panic.h"
#include "process.h"

extern void (*__init_array[])();
extern void (*__init_array_end[])();

/* Startup Beep*/
void StartUp_Beeps() {
    startbeep(450);
    pit_sleep_ms(100);
    mutebeep();
    startbeep(775);
    pit_sleep_ms(50);
    mutebeep();
}

/* A Simple kernel written in C
 * These parameters are pushed onto the stack by the assembly kernel entry file.
 */

void term_log_ok(const char* message) {
    term_write("   OK   ", TC_LIME);
    term_write(message, TC_WHITE);
}
void term_log_error(const char* message) {
    term_write("  ERROR ", TC_LRED);
    term_write(message, TC_WHITE);
}
void term_log_critical(const char* message) {
    term_write("  CRIT  ", TC_DKRED);
    term_write(message, TC_LRED);
}
void term_log_info(const char* message) {
    term_write("  INFO  ", TC_LBLUE);
    term_write(message, TC_WHITE);
}

void k_main(multiboot_info_t* mbd, uint32_t magic) {
    gdt_init();
    idt_init();
    kmalloc_init();

    vga_text_init(TC_BLACK);

    u32 term_width = VGA_width;               // Set this to the width of your VGA text mode
    u32 visible_height = VGA_height;          // Set this to the height of your VGA text mode (visible height)
    u32 buffer_height = visible_height + 100; // Set this to a height larger than visible to allow scrolling

    term_init(term_width, buffer_height, visible_height, vga_set_char, vga_move_cursor);

    // Logging previous events
    term_log_info("Choacury Kernel Booting...\n");
    term_log_ok("Initialized GDT\n");
    term_log_ok("Initialized IDT\n");
    term_log_ok("Initialized Kernel Heap\n");
    term_log_ok("Initialized terminal\n");

    // Moved down
    /*term_write("\n\xB0\xB1\xB2\xDB Welcome to Choacury! \xDB\xB2\xB1\xB0\n", TC_LIME);
    term_write("Version: Build " __DATE__ " (GUI Testing)\n", TC_WHITE);
    term_write("(C)opyright: \2 Pineconium 2023-2025.\n\n", TC_WHITE);*/

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        //panic("Bootloader did not provide multiboot information\n");
        term_log_critical("Bootloader did not provide multiboot information\n");
    }

    // For the stuff on lines 30-31
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++)
    {
        __init_array[i]();
    }

    pmm_init(mbd);
    term_log_ok("Initialized PMM\n");
    pic_init();
    term_log_ok("Initialized PIC\n");
    pit_init();
    term_log_ok("Initialized PIT\n");

    asm volatile("sti");

    ps2_init();
    ps2_init_keymap_us();

    term_log_ok("Initialized PS2\n");

    // StartUp_Beeps();

    storage_device_init();
    debug_print_pci();

    term_log_ok("Initialized storage device\n");

    term_log_info("Clearing processes\n");
    for(int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].state = PROCESS_TERMINATED;
    }
    term_log_ok("Cleared processes successfully\n");

    /*term_log_info("Would you like to start with graphics or shell? (g/s)\n");
    uint8_t init_shell = 0;
    for (;;)
    {
        key_event_t key_event;
        ps2_get_key_event(&key_event);

        if(key_event.key == KEY_G) {
            init_shell = 0;
            break;
        } else if(key_event.key == KEY_S) {
            init_shell = 1;
            break;
        }
    }

    if(init_shell == 0) {
        term_log_info("Initializing GUI...\n");

        panic("Couldn't load LNDE\n");
    }*/

    term_log_info("Initializing shell...\n");

    term_write("\n\xB0\xB1\xB2\xDB Welcome to Choacury! \xDB\xB2\xB1\xB0\n", TC_LIME);
    term_write("Version: Build " __DATE__ " (GUI Testing)\n", TC_WHITE);
    term_write("(C)opyright: \2 Pineconium 2023-2025.\n\n", TC_WHITE);

    create_process(&shell_start, HIGH_PRIORITY);
    scheduler();

    shell_start();
}
