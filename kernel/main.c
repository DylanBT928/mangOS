#include <kernel.h>
#include <limine.h>
#include <stdbool.h>
#include <stddef.h>

#include "drivers/framebuffer.h"
#include "drivers/serial.h"
#include "libc/printf.h"

// This is our Kernel's main entry point
// If we rename this function, we have to change the linker script accordingly
void kmain(void)
{
    serial_init();
    serial_printf("Hello from mangoOS!\n");

    framebuffer_init();

    fb_clear(0x8ace00);

    fb_draw_char_transparent('m', 552, 370, 0x000000, 2);
    fb_draw_char_transparent('a', 576, 370, 0x000000, 2);
    fb_draw_char_transparent('n', 600, 370, 0x000000, 2);
    fb_draw_char_transparent('g', 624, 370, 0x000000, 2);
    fb_draw_char_transparent('o', 648, 370, 0x000000, 2);
    fb_draw_char_transparent('O', 672, 370, 0x000000, 2);
    fb_draw_char_transparent('S', 696, 370, 0x000000, 2);

    fb_draw_string_transparent("Best Operating System", 464, 424, 0x000000, 2);

    hcf();
}
