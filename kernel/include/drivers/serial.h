#pragma once

#include <stdint.h>

// Serial log functions
char *get_serial_log();

// Core serial functions
void serial_init();
void serial_putchar(char c);
void serial_print(const char* str);

// Extended output functions
void serial_print_hex(uint64_t num);
void serial_print_dec(int64_t num);
void serial_print_bin(uint64_t num);
void serial_printf(const char *fmt, ...);

// Terminal control
void serial_clearchar();