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

    for (size_t i = 0; i < 100; i++)
    {
        fb_put_pixel(i, i, 0xffffff);
    }

    hcf();
}
