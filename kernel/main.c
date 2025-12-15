#include <kernel.h>
#include <limine.h>

// This is our Kernel's main entry point
// If we rename this function, we have to change the linker script accordingly
void kmain(void)
{
    serial_init();
    serial_printf("Hello from mangOS!\n");

    terminal_init();

    terminal_font_increase_scale();

    // hello mango
    terminal_putc('h');
    terminal_putc('e');
    terminal_putc('l');
    terminal_putc('l');
    terminal_putc('o');
    terminal_putc(' ');
    terminal_putc('m');
    terminal_putc('a');
    terminal_putc('n');
    terminal_putc('g');
    terminal_putc('o');

    // test newline character (\n)
    terminal_putc('\n');
    terminal_putc('\n');
    terminal_putc('a');
    terminal_putc('\n');
    terminal_putc('b');
    terminal_putc('\n');
    terminal_putc('\n');

    // hello string
    terminal_write("hello string\n\n");

    // check terminal scrolling
    // for (char c = 'A'; c <= 'z'; ++c)
    // {
    //     terminal_putc(c);
    //     // terminal_putc('\n');
    // }

    for (size_t i = 0; i < 100; ++i)
    {
        terminal_putc('a');
        terminal_putc('\n');
    }

    // terminal_putc('\n');
    //
    // for (size_t i = 0; i < 200; ++i)
    // {
    //     terminal_putc('c');
    // }

    terminal_write("if you see this, terminal scrolling works!");

    hcf();
}
