#include "cpu/gdt.h"

#include "cpu/tss.h"
#include "drivers/serial.h"
#include "libc/string.h"

// GDT has 7 entries total:
// 0: Null descriptor
// 1: Kernel code segment
// 2: Kernel data segment
// 3: User code segment
// 4: User data segment
// 5-6: TSS (Task State Segment) - takes 2 entries in 64-bit mode
#define GDT_ENTRIES 7

gdt_entry_tss gdt[GDT_ENTRIES];
gdt_register gdtr;

void gdt_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    gdt[i].limit = limit & 0xFFFF;        // Lower 16 bits of limit
    gdt[i].flags = (limit >> 16) & 0x0F;  // Upper 4 bits of limit

    gdt[i].base_low = base & 0xFFFF;         // Lower 16 bits of base
    gdt[i].base_mid = (base >> 16) & 0xFF;   // Middle 8 bits of base
    gdt[i].base_high = (base >> 24) & 0xFF;  // Upper 8 bits of base

    gdt[i].access_byte = access;

    // Combine flags (upper 4 bits) with upper limit bits (lower 4 bits)
    gdt[i].flags |= (flags & 0xF0);
}

void init_gdt()
{
    // Clear the GDT
    memset(&gdt, 0, sizeof(gdt));

    gdt_set_entry(0, 0, 0, 0, 0);              // Null descriptor
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xA0);  // Kernel Code Segment (selector 0x08)
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xA0);  // Kernel Data Segment (selector 0x10)
    gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xA0);  // User Code Segment (selector 0x18)
    gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xA0);  // User Data Segment (selector 0x20

    init_tss();

    // Set up the GDT register (GDTR)
    gdtr.limit = (sizeof(gdt_entry_tss) * GDT_ENTRIES) - 1;
    gdtr.base = (uint64_t)&gdt;

    serial_printf("Loading GDT at: base=0x%p, limit=0x%x\n", (void*)gdtr.base, gdtr.limit);

    load_gdt(&gdtr);
    load_tss();

    serial_printf("Loaded GDT and TSS successfully.");
}
