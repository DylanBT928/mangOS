#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>
#include <stddef.h>

#define vsnprintf vsnprintf_

int vsnprintf(char* str, size_t buff_size, const char* format, va_list argptr);

#endif
