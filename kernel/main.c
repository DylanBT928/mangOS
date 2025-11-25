#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests"))) static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine__requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine__requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void* memcpy(void* restrict dest, const void* restrict src, size_t n)
{
    uint8_t* restrict pdest = (uint8_t* restrict)dest;
    const uint8_t* restrict psrc = (const uint8_t* restrict)src;

    for (size_t i = 0; i < n; ++i)
    {
        pdest[i] = psrc[i];
    }

    return dest;
}

void* memset(void* s, int c, size_t n)
{
    uint8_t* p = (uint8_t*)s;

    for (size_t i = 0; i < n; ++i)
    {
        p[i] = (uint8_t)c;
    }

    return s;
}

void* memmove(void* dest, const void* src, size_t n)
{
    uint8_t* pdest = (uint8_t*)dest;
    const uint8_t* psrc = (const uint8_t*)src;

    if (src > dest)
    {
        for (size_t i = 0; i < n; ++i)
        {
            pdest[i] = psrc[i];
        }
    }
    else if (src < dest)
    {
        for (size_t i = n; i > 0; --i)
        {
            pdest[i - 1] = psrc[i - 1];
        }
    }

    return dest;
}

int memcmp(const void* s1, const void* s2, size_t n)
{
    const uint8_t* p1 = (const uint8_t*)s1;
    const uint8_t* p2 = (const uint8_t*)s2;

    for (size_t i = 0; i < n; ++i)
    {
        if (p1[i] != p2[i])
        {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// Halt and catch fire
static void hcf(void)
{
    for (;;)
    {
        asm("hlt");
    }
}

void kmain(void)
{
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
    {
        hcf();
    }

    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    struct limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];

    for (size_t i = 0; i < 100; ++i)
    {
        volatile uint32_t* fb_ptr = fb->address;
        fb_ptr[i * (fb->pitch / 4) + i] = 0xffffff;
    }

    hcf();
}
