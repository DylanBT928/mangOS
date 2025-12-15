#include <kernel.h>

// This is our Kernel's main entry point
// If we rename this function, we have to change the linker script accordingly

void kmain(void)
{
    serial_init();
    serial_printf("Hello from mangOS!\n");

    terminal_init();

    terminal_font_increase_scale();

    terminal_write("Welcome to ", WHITE);
    terminal_write("mangOS", YELLOW);
    terminal_write("!\n", WHITE);

    terminal_putc('\n', WHITE);
    terminal_write(itoa(255, 10), WHITE);  // 255

    terminal_putc('\n', WHITE);
    terminal_write(itoa(255, 16), WHITE);  // ff

    terminal_putc('\n', WHITE);
    terminal_write(itoa(255, 2), WHITE);  // 11111111

    terminal_putc('\n', WHITE);
    terminal_write(itoa(10, 16), WHITE);  // a

    hcf();
}
