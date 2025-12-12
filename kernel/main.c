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

    fb_draw_string("Welcome to MangoOS!", 0, 0, 0xffffff, 2);

    hcf();
}
