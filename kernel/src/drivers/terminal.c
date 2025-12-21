#include "drivers/terminal.h"

#include <limine.h>
#include <stdint.h>

#include "colors.h"
#include "drivers/framebuffer.h"
#include "libc/string.h"

static const uint8_t font_width = 8;
static const uint8_t font_height = 8;

static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

static uint16_t term_width;
static uint16_t term_height;

static uint8_t font_scale = 1;

static uint16_t char_w;
static uint16_t char_h;

void terminal_init()
{
    framebuffer_init();
    fb_clear(BLACK);

    term_width = fb->width / font_width;
    term_height = fb->height / font_height;

    char_w = font_width * font_scale;
    char_h = font_height * font_scale;
}

void terminal_putc(char c, uint32_t color)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y += char_h;
    }
    else if (cursor_x + char_w > fb->width)
    {
        cursor_x = 0;
        cursor_y += char_h;
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
    for (size_t i = 0; str[i]; ++i)
    {
        terminal_putc(str[i], color);
    }
}

void terminal_scroll()
{
    int char_h = font_height * font_scale;

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
    if (font_scale < 8)
    {
        ++font_scale;

        char_w = font_width * font_scale;
        char_h = font_height * font_scale;
    }
}

void terminal_font_decrease_scale()
{
    if (font_scale > 1)
    {
        --font_scale;

        char_w = font_width * font_scale;
        char_h = font_height * font_scale;
    }
}
