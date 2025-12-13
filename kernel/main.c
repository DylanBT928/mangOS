#include <kernel.h>
#include <limine.h>

// This is our Kernel's main entry point
// If we rename this function, we have to change the linker script accordingly
void kmain(void)
{
    serial_init();
    serial_printf("Hello from mangOS!\n");

    framebuffer_init();

    fb_draw_string("Welcome to MangOS!", 0, 0, 0xFFFFFF, 0x000000, 2);

    hcf();
}
