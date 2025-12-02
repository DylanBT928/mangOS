#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include <stdint.h>

char* serial_get_log();
void serial_init();
void serial_write_char(char c);
void serial_print(const char* str);
void serial_printf(const char* fmt, ...);
void serial_clear_char();

#endif
