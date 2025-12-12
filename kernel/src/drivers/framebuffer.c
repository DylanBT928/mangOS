#include "drivers/framebuffer.h"

#include <kernel.h>
#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drivers/serial.h"
#include "font.h"

// DO NOT CHANGE -- PLEASE CONSULT BEFORE CHANGING ------------------------------------

// Sets the base revision to version number 4 to insure compatibility between kernal
// and the bootloader. Will help to overcome shortcomings of older boot protocals and
// provides good access to 64_bit long mode and other modern niceties
// DO NOT CHANGE BASE REVISION: ALL OTHER VERSIONS ARE DEPRECATED OUR CODE WILL
__attribute__((used, section(".limine_requests"))) static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

// Most Limine requests should be made volatile so the compiler does not optimize them
// away. They should also be accessed at least once or marked with "used" attribute
// The framebuffer is a portion of ram that stores image data for video display, we
// need this for any kind of image to be accessed
__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// Here we set the start and end markers for the Limine requests
// these can be moved anywhere, but just dont they work right now
__attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine__requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine__requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// Store the active framebuffer info
static struct limine_framebuffer* fb = NULL;

// Halt and catch fire
// This function leads to a complete system crash, calling inline "HALT" infinite times
// This is a pretty unsafe function thats used for some debugging
// NEVER CALL THIS UNLESS YOU ABSOLUTELY NEED TO
void hcf(void)
{
    for (;;)
    {
        asm("hlt");
    }
}

void framebuffer_init(void)
{
    // Ensures the bootloader actually understands our base revision
    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
    {
        // error-catching (crashing our os if it doesnt work lol)
        serial_printf("error: unsupported Limine base revision");
        hcf();
    }

    // Ensures our framebuffer works
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        serial_printf("error: framebuffer request failed");
        hcf();
    }

    // We fetch the first framebuffer (only one by default) check documentation
    // for the struct layout request -> response -> framebuffer
    fb = framebuffer_request.response->framebuffers[0];
}

void fb_put_pixel(int x, int y, uint32_t color)
{
    // Check if fb is initialized to avoid null pointer dereference
    if (fb == NULL)
    {
        return;
    }

    uint64_t offset = y * fb->pitch + x * (fb->bpp / 8);
    uint32_t* pixel_addr = (uint32_t*)((uint8_t*)fb->address + offset);

    *pixel_addr = color;
}

void fb_clear(uint32_t color)
{
    // Check if fb is initialized to avoid null pointer dereference
    if (fb == NULL)
    {
        return;
    }

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    uint8_t bytes_per_pixel = fb->bpp / 8;
    uint8_t* where = (uint8_t*)fb->address;

    for (uint64_t y = 0; y < fb->height; ++y)
    {
        for (uint64_t x = 0; x < fb->width; ++x)
        {
            where[x * bytes_per_pixel] = b;
            where[x * bytes_per_pixel + 1] = g;
            where[x * bytes_per_pixel + 2] = r;
        }

        where += fb->pitch;
    }
}

void fb_fill_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color)
{
    if (fb == NULL)
    {
        return;
    }

    if (x + width > fb->width)
    {
        width = fb->width - x;
    }

    if (y + height > fb->height)
    {
        height = fb->height - y;
    }

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    uint8_t bytes_per_pixel = fb->bpp / 8;
    uint8_t* where = (uint8_t*)fb->address + (y * fb->pitch) + (x * bytes_per_pixel);

    for (uint64_t row = 0; row < height; ++row)
    {
        for (uint64_t col = 0; col < width; ++col)
        {
            where[col * bytes_per_pixel] = b;
            where[col * bytes_per_pixel + 1] = g;
            where[col * bytes_per_pixel + 2] = r;
        }

        where += fb->pitch;
    }
}

void fb_draw_char(char c, int x, int y, uint32_t fg_color, uint32_t bg_color)
{
    if (fb == NULL)
    {
        return;
    }

    int font_idx = (uint8_t)c;

    for (int row = 0; row < 8; ++row)
    {
        uint8_t bitmap_row = font8x8_basic[font_idx][row];

        for (int col = 0; col < 8; ++col)
        {
            bool pixel_set = (bitmap_row >> col) & 1;

            if (pixel_set)
            {
                fb_put_pixel(x + col, y + row, fg_color);
            }
            else
            {
                fb_put_pixel(x + col, y + row, bg_color);
            }
        }
    }
}

void fb_draw_char_transparent(char c, int x, int y, uint32_t color, int scale)
{
    if (fb == NULL)
    {
        return;
    }

    int font_idx = (uint8_t)c;

    for (int row = 0; row < 8; ++row)
    {
        uint8_t bitmap_row = font8x8_basic[font_idx][row];

        for (int col = 0; col < 8; ++col)
        {
            bool pixel_set = (bitmap_row >> col) & 1;

            if (pixel_set)
            {
                for (int scale_y = 0; scale_y < scale; ++scale_y)
                {
                    for (int scale_x = 0; scale_x < scale; ++scale_x)
                    {
                        fb_put_pixel(x + (col * scale) + scale_x, y + (row * scale) + scale_y, color);
                    }
                }
            }
        }
    }
}

void fb_draw_string_transparent(const char* str, int x, int y, uint32_t color, int scale)
{
    if (fb == NULL || str == NULL)
    {
        return;
    }

    int cursor_x = x;
    int char_width = 8 * scale;

    while (*str)
    {
        fb_draw_char_transparent(*str, cursor_x, y, color, scale);
        cursor_x += char_width;
        ++str;
    }
}
