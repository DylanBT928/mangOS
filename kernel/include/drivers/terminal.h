#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

static size_t term_width;
static size_t term_height;

static size_t term_row;
static size_t term_col;

void terminal_init();
void terminal_putc(char c);
void terminal_write();
void terminal_scroll();

#endif
