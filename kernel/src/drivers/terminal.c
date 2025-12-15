#include "drivers/terminal.h"

#include <limine.h>

#include "colors.h"
#include "drivers/framebuffer.h"

void terminal_init()
{
    framebuffer_init();
    fb_clear(BLACK);

    term_width = fb->width / FONT_WIDTH;
    term_height = fb->height / FONT_HEIGHT;

    term_row = 0;
    term_col = 0;

    font_scale = 1;
}

void terminal_putc(char c)
{
    if (term_row == fb->width || c == '\n')
    {
        term_col += FONT_HEIGHT * font_scale;
        term_row = 0;
    }
    else
    {
        fb_draw_char(c, term_row, term_col, WHITE, BLACK, font_scale);
        term_row += FONT_WIDTH * font_scale;
    }
}

void terminal_write()
{
}

void terminal_scroll()
{
}

void terminal_font_increase_scale()
{
    ++font_scale;
}

void terminal_font_decrease_scale()
{
    --font_scale;
}
