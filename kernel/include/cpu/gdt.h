#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef union gdt_entry 
{
    struct {
        uint16_t limit;
        uint16_t base_low;
        uint8_t  base_mid;
        uint8_t  access_byte;
        uint8_t  flags;
        uint8_t  base_high;
    };
    struct {
        uint32_t base_upper;
        uint32_t reserved;
    };

} __attribute__((packed)) gdt_entry_tss;


typedef struct 
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_register;

void init_gdt();
void gdt_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

#endif