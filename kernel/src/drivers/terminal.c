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

    cursor_x = 0;
    cursor_y = 0;

    font_scale = 1;
}

void terminal_putc(char c)
{
    int char_w = FONT_WIDTH * font_scale;
    int char_h = FONT_HEIGHT * font_scale;

    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y += char_h;
    }
    else
    {
        if (cursor_x > fb->width)
        {
            cursor_x = 0;
            cursor_y += char_h;
        }

        fb_draw_char(c, cursor_x, cursor_y, WHITE, BLACK, font_scale);
        cursor_x += char_w;
    }

    if (cursor_y > fb->height)
    {
        terminal_scroll();
    }
}

void terminal_write(const char* str)
{
    size_t i = 0;

    while (str[i] != '\0')
    {
        terminal_putc(str[i++]);
    }
}

void terminal_scroll()
{
    int char_h = FONT_HEIGHT * font_scale;

    uint8_t* base = (uint8_t*)fb->address;
    uint64_t row_bytes = fb->pitch;
    uint64_t copy_bytes = (fb->height - char_h) * row_bytes;

    for (uint64_t i = 0; i < copy_bytes; ++i)
    {
        base[i] = base[i + char_h * row_bytes];
    }

    fb_fill_rect(0, fb->height - char_h, fb->width, char_h, BLACK);

    cursor_x = 0;
    cursor_y = fb->height - char_h;
}

void terminal_font_increase_scale()
{
    ++font_scale;
}

void terminal_font_decrease_scale()
{
    --font_scale;
}
