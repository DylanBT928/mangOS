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

    terminal_write("Welcome to ", WHITE);
    terminal_write("mangOS", YELLOW);
    terminal_write("!\n", WHITE);

    hcf();
}
