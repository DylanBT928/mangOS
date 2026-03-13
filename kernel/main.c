#include <kernel.h>

// This is our Kernel's main entry point
// If we rename this function, we have to change the linker script accordingly

void kmain(void)
{
    serial_init();
    serial_printf("Hello from mangOS!\n");

    init_gdt();

    terminal_init();

    terminal_font_increase_scale();
    terminal_putc('\n', WHITE);

    terminal_write("  ▄▄▄▄███▄▄▄▄      ▄████████ ███▄▄▄▄      ▄██████▄   ▄██████▄     ▄████████\n", YELLOW);
    terminal_write("▄██▀▀▀███▀▀▀██▄   ███    ███ ███▀▀▀██▄   ███    ███ ███    ███   ███    ███\n", YELLOW);
    terminal_write("███   ███   ███   ███    ███ ███   ███   ███    █▀  ███    ███   ███    █▀ \n", YELLOW);
    terminal_write("███   ███   ███   ███    ███ ███   ███  ▄███        ███    ███   ███       \n", YELLOW);
    terminal_write("███   ███   ███ ▀███████████ ███   ███ ▀▀███ ████▄  ███    ███ ▀███████████\n", YELLOW);
    terminal_write("███   ███   ███   ███    ███ ███   ███   ███    ███ ███    ███          ███\n", YELLOW);
    terminal_write("███   ███   ███   ███    ███ ███   ███   ███    ███ ███    ███    ▄█    ███\n", YELLOW);
    terminal_write(" ▀█   ███   █▀    ███    █▀   ▀█   █▀    ████████▀   ▀██████▀   ▄████████▀ \n", YELLOW);

    terminal_putc('\n', WHITE);

    terminal_write("\tOS: mangOS\n", WHITE);
    terminal_write("\tKernel: x64\n", WHITE);
    terminal_write("\tVersion: 0.0.1\n", WHITE);

    terminal_putc('\n', WHITE);

    terminal_write("Type \'help\' to get a list of available commands.\n", WHITE);
    terminal_write("mangOS> ", YELLOW);

    hcf();
}
