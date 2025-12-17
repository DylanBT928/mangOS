#include "drivers/terminal.h"

#include <limine.h>
#include <stdint.h>

#include "libc/string.h"
#include "colors.h"
#include "drivers/framebuffer.h"

static size_t term_width;
static size_t term_height;

static size_t cursor_x;
static size_t cursor_y;

static size_t font_scale;

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

void terminal_putc(char c, uint32_t color)
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
        if (cursor_x + char_w > fb->width)
        {
            cursor_x = 0;
            cursor_y += char_h;
        }
    }

    if (cursor_y + char_h > fb->height)
    {
        terminal_scroll();
    }

    if (c != '\n')
    {
        fb_draw_char(c, cursor_x, cursor_y, color, BLACK, font_scale);
        cursor_x += char_w;
    }
}

void terminal_write(const char* str, uint32_t color)
{
    size_t i = 0;

    while (str[i] != '\0')
    {
        terminal_putc(str[i++], color);
    }
}

void terminal_scroll()
{
    int char_h = FONT_HEIGHT * font_scale;

    uint8_t* base = (uint8_t*)fb->address;
    uint64_t row_bytes = fb->pitch;
    uint64_t offset = (uint64_t)char_h * row_bytes;
    uint64_t copy_bytes = (uint64_t)(fb->height - char_h) * row_bytes;

    memmove(base, base + offset, copy_bytes);

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
