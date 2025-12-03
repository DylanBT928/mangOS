#include "libc/printf.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NTOA_BUFFER_SIZE 32

#define FLAGS_ZEROPAD (1U << 0U)
#define FLAGS_LEFT (1U << 1U)
#define FLAGS_PLUS (1U << 2U)
#define FLAGS_SPACE (1U << 3U)
#define FLAGS_HASH (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_LONG (1U << 6U)
#define FLAGS_LONG_LONG (1U << 7U)

typedef void (*out_fct_type)(char character, void* buffer, size_t idx, size_t maxlen);

static inline void _out_buffer(char character, void* buffer, size_t idx, size_t maxlen)
{
    if (idx < maxlen)
    {
        ((char*)buffer)[idx] = character;
    }
}

static inline bool _is_digit(char ch)
{
    return (ch >= '0') && (ch <= '9');
}

static unsigned int _atoi(const char** str)
{
    unsigned int i = 0U;

    while (_is_digit(**str))
    {
        i = i * 10U + (unsigned int)(*((*str)++) - '0');
    }

    return i;
}

static inline unsigned int _strnlen_s(const char* str, size_t maxsize)
{
    const char* s;

    for (s = str; *s && maxsize--; ++s);

    return (unsigned int)(s - str);
}

static size_t _out_rev(out_fct_type out, char* buffer, size_t idx, size_t maxlen, const char* buf, size_t len, unsigned int width, unsigned int flags)
{
    const size_t start_idx = idx;

    if (!(flags & (FLAGS_LEFT | FLAGS_ZEROPAD)))
    {
        for (size_t i = len; i < width; i++)
        {
            out(' ', buffer, idx++, maxlen);
        }
    }

    while (len)
    {
        out(buf[--len], buffer, idx++, maxlen);
    }

    if (flags & FLAGS_LEFT)
    {
        while (idx - start_idx < width)
        {
            out(' ', buffer, idx++, maxlen);
        }
    }

    return idx;
}

static size_t _ntoa_format(out_fct_type out, char* buffer, size_t idx, size_t maxlen,
                           char* buf, size_t len, bool negative, unsigned int base,
                           unsigned int width, unsigned int flags)
{
    if (!(flags & FLAGS_LEFT))
    {
        if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE))))
        {
            --width;
        }

        while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < NTOA_BUFFER_SIZE))
        {
            buf[len++] = '0';
        }
    }

    if (flags & FLAGS_HASH)
    {
        if ((base == 16U) && (len < NTOA_BUFFER_SIZE))
        {
            buf[len++] = (flags & FLAGS_UPPERCASE) ? 'X' : 'x';
        }

        if (len < NTOA_BUFFER_SIZE)
        {
            buf[len++] = '0';
        }
    }

    if (len < NTOA_BUFFER_SIZE)
    {
        if (negative)
        {
            buf[len++] = '-';
        }
        else if (flags & FLAGS_PLUS)
        {
            buf[len++] = '+';
        }
        else if (flags & FLAGS_SPACE)
        {
            buf[len++] = ' ';
        }
    }

    return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

static size_t _ntoa_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen,
                         unsigned long value, bool negative, unsigned long base,
                         unsigned int width, unsigned int flags)
{
    char buf[NTOA_BUFFER_SIZE];
    size_t len = 0U;

    if (!value)
    {
        flags &= ~FLAGS_HASH;
    }
    else
    {
        while (value && (len < NTOA_BUFFER_SIZE))
        {
            const char digit = (char)(value % base);

            buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        }
    }

    return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, width, flags);
}

static size_t _ntoa_long_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen,
                              unsigned long long value, bool negative, unsigned long long base,
                              unsigned int width, unsigned int flags)
{
    char buf[NTOA_BUFFER_SIZE];
    size_t len = 0U;

    if (!value)
    {
        flags &= ~FLAGS_HASH;
    }

    if (value)
    {
        while (value && (len < NTOA_BUFFER_SIZE))
        {
            const char digit = (char)(value % base);

            buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        }
    }

    return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, width, flags);
}

static int _vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* format, va_list va)
{
    unsigned int flags, width, n;
    size_t idx = 0U;

    while (*format)
    {
        if (*format != '%')
        {
            out(*format, buffer, idx++, maxlen);
            ++format;
            continue;
        }

        ++format;
        flags = 0U;

        while (n)
        {
            switch (*format)
            {
                case '0':
                {
                    flags |= FLAGS_ZEROPAD;
                    ++format;
                    n = 1U;
                    break;
                }

                case '-':
                {
                    flags |= FLAGS_LEFT;
                    ++format;
                    n = 1U;
                    break;
                }

                case '+':
                {
                    flags |= FLAGS_PLUS;
                    ++format;
                    n = 1U;
                    break;
                }

                case ' ':
                {
                    flags |= FLAGS_SPACE;
                    ++format;
                    n = 1U;
                    break;
                }

                case '#':
                {
                    flags |= FLAGS_HASH;
                    ++format;
                    n = 1U;
                    break;
                }

                default:
                {
                    n = 0U;
                    break;
                }
            }
        }

        width = 0U;

        if (_is_digit(*format))
        {
            width = _atoi(&format);
        }

        switch (*format)
        {
            case 'l':
            {
                flags |= FLAGS_LONG;
                ++format;

                if (*format == 'l')
                {
                    flags |= FLAGS_LONG_LONG;
                    ++format;
                }

                break;
            }

            case 'z':
            {
                flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
                ++format;
                break;
            }

            default:
            {
                break;
            }
        }

        switch (*format)
        {
            case 'd':
            case 'i':
            case 'u':
            case 'x':
            case 'X':
            {
                unsigned int base;

                if (*format == 'x' || *format == 'X')
                {
                    base = 16U;
                }
                else
                {
                    base = 10U;
                    flags &= ~FLAGS_HASH;
                }

                if (*format == 'X')
                {
                    flags |= FLAGS_UPPERCASE;
                }

                if (*format == 'u' || *format == 'x' || *format == 'X')
                {
                    flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
                }

                if ((*format == 'i') || (*format == 'd'))
                {
                    if (flags & FLAGS_LONG_LONG)
                    {
                        const long long value = va_arg(va, long long);
                        idx = _ntoa_long_long(out, buffer, idx, maxlen,
                                              (unsigned long long)(value > 0 ? value : 0 - value),
                                              value < 0, base, width, flags);
                    }
                    else if (flags & FLAGS_LONG)
                    {
                        const long value = va_arg(va, long);
                        idx = _ntoa_long(out, buffer, idx, maxlen,
                                         (unsigned long)(value > 0 ? value : 0 - value),
                                         value < 0, base, width, flags);
                    }
                    else
                    {
                        const int value = va_arg(va, int);
                        idx = _ntoa_long(out, buffer, idx, maxlen,
                                         (unsigned int)(value > 0 ? value : 0 - value),
                                         value < 0, base, width, flags);
                    }
                }
                else
                {
                    if (flags & FLAGS_LONG_LONG)
                    {
                        idx = _ntoa_long_long(out, buffer, idx, maxlen,
                                              va_arg(va, unsigned long long),
                                              false, base, width, flags);
                    }
                    else if (flags & FLAGS_LONG)
                    {
                        idx = _ntoa_long(out, buffer, idx, maxlen,
                                         va_arg(va, unsigned long),
                                         false, base, width, flags);
                    }
                    else
                    {
                        idx = _ntoa_long(out, buffer, idx, maxlen,
                                         va_arg(va, unsigned int),
                                         false, base, width, flags);
                    }
                }

                ++format;
                break;
            }

            case 'c':
            {
                unsigned int l = 1U;

                if (!(flags & FLAGS_LEFT))
                {
                    while (l++ < width)
                    {
                        out(' ', buffer, idx++, maxlen);
                    }
                }

                out((char)va_arg(va, int), buffer, idx++, maxlen);

                if (flags & FLAGS_LEFT)
                {
                    while (l++ < width)
                    {
                        out(' ', buffer, idx++, maxlen);
                    }
                }

                ++format;
                break;
            }

            case 's':
            {
                const char* p = va_arg(va, char*);

                if (!p)
                {
                    p = "(null)";
                }

                unsigned int l = _strnlen_s(p, (size_t)-1);

                if (!(flags & FLAGS_LEFT))
                {
                    while (l++ < width)
                    {
                        out(' ', buffer, idx++, maxlen);
                    }
                }

                while (*p)
                {
                    out(*(p++), buffer, idx++, maxlen);
                }

                if (flags & FLAGS_LEFT)
                {
                    while (l++ < width)
                    {
                        out(' ', buffer, idx++, maxlen);
                    }
                }

                ++format;
                break;
            }

            case 'p':
            {
                width = sizeof(void*) * 2U;
                flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;

                const bool is_ll = sizeof(uintptr_t) == sizeof(long long);

                if (is_ll)
                {
                    idx = _ntoa_long_long(out, buffer, idx, maxlen,
                                          (uintptr_t)va_arg(va, void*),
                                          false, 16U, width, flags);
                }
                else
                {
                    idx = _ntoa_long(out, buffer, idx, maxlen,
                                     (unsigned long)((uintptr_t)va_arg(va, void*)),
                                     false, 16U, width, flags);
                }

                ++format;
                break;
            }

            case '%':
            {
                out('%', buffer, idx++, maxlen);
                ++format;
                break;
            }

            default:
            {
                out(*format, buffer, idx++, maxlen);
                ++format;
                break;
            }
        }
    }

    out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

    return (int)idx;
}

int vsnprintf(char* buffer, size_t maxlen, const char* format, va_list va)
{
    return _vsnprintf(_out_buffer, buffer, maxlen, format, va);
}
