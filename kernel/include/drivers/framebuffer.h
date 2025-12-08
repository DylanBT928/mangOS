#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void *address;              
    uint64_t width;             
    uint64_t height;            
    uint64_t pitch;             
    uint16_t bits_per_pixel;               
    uint8_t bytes_per_pixel;     
} framebuffer_t;

bool framebuffer_init(void);

void fb_put_pixel(int x, int y, uint32_t color);
void fb_clear(uint32_t color);
void fb_fill_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color);
void fb_draw_char(char c, int x, int y, uint32_t fg_color, uint32_t bg_color);
void fb_draw_string(const char *str, int x, int y, uint32_t fg_color, uint32_t bg_color);

#endif