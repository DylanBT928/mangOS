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

    fb_clear(0xFF00FF);

    fb_draw_string_transparent("Welcome to MangoOS!", 0, 0, 0x000000, 2);
    fb_fill_rect(600, 300, 100, 100, 0x00FF00);
    fb_fill_rect(620, 320, 20, 20, 0x0000FF);
    fb_fill_rect(660, 320, 20, 20, 0x0000FF);
    fb_fill_rect(620, 370, 20, 20, 0xFF0000);
    fb_fill_rect(640, 370, 20, 20, 0xFF0000);
    fb_fill_rect(660, 370, 20, 20, 0xFF0000);

    hcf();
}
