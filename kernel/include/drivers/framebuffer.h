#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    void* address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
} framebuffer_t;

void framebuffer_init(void);
void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void fb_clear(uint32_t color);
void fb_fill_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color);
void fb_draw_char(char c, int x, int y, uint32_t fg_color, uint32_t bg_color, int scale);
void fb_draw_string(const char* str, int x, int y, uint32_t fg_color, uint32_t bg_color, int scale);

#endif
