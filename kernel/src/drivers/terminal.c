#include "drivers/terminal.h"

#include <limine.h>
#include <stdint.h>

#include "colors.h"
#include "drivers/framebuffer.h"
#include "libc/string.h"

static const uint8_t font_width = 8;
static const uint8_t font_height = 16;

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

void terminal_putc(uint32_t c, uint32_t color)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y += char_h;
        return;
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

    fb_draw_char(c, cursor_x, cursor_y, color, BLACK, font_scale);
    cursor_x += char_w;
}

void terminal_write(const char* str, uint32_t color)
{
    uint8_t* ustr = (uint8_t*)str;

    while (*ustr)
    {
        uint32_t c = 0;

        if ((*ustr & 0x80) == 0)
        {
            // 1-byte
            c = *ustr++;
        }
        else if ((*ustr & 0xE0) == 0xC0)
        {
            // 2-byte
            c = ((*ustr & 0x1F) << 6) | (*(ustr + 1) & 0x3F);
            ustr += 2;
        }
        else if ((*ustr & 0xF0) == 0xE0)
        {
            // 3-byte
            c = ((*ustr & 0x0F) << 12) | ((*(ustr + 1) & 0x3F) << 6) | (*(ustr + 2) & 0x3F);
            ustr += 3;
        }
        else
        {
            // unsupported or 4-byte
            ++ustr;
            continue;
        }

        terminal_putc(c, color);
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
