#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

void terminal_init();
void terminal_putc(char c, uint32_t color);
void terminal_write(const char* str, uint32_t color);
void terminal_scroll();

void terminal_font_increase_scale();
void terminal_font_decrease_scale();

#endif
