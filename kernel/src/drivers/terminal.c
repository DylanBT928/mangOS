#include "drivers/terminal.h"

#include <limine.h>

#include "colors.h"
#include "drivers/framebuffer.h"

void terminal_init()
{
    fb_clear(BLACK);

    term_width = fb->width / FONT_WIDTH;
    term_height = fb->height / FONT_HEIGHT;

    term_row = 0;
    term_col = 0;
}

void terminal_putc(char c)
{
}

void terminal_write()
{
}

void terminal_scroll()
{
}
