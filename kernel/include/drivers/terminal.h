#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

static size_t term_width;
static size_t term_height;

static size_t cursor_x;
static size_t cursor_y;

static size_t font_scale;

void terminal_init();
void terminal_putc(char c);
void terminal_write(const char* str);
void terminal_scroll();

void terminal_font_increase_scale();
void terminal_font_decrease_scale();

#endif
