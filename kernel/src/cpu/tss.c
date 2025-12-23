#include "cpu/tss.h"

#include "cpu/gdt.h"
#include "drivers/serial.h"
#include "libc/string.h"

#define GDT_ENTRIES 7
#define STACK_SIZE 0x4000

static uint8_t kernel_stack[STACK_SIZE] __attribute__((aligned(16)));

extern gdt_entry_tss gdt[GDT_ENTRIES];

static tss_entry tss;

void tss_set_entry(int i, uint64_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    // First 8 bytes (standard descriptor format)
    gdt[i].limit = (limit & 0xFFFF);
    gdt[i].flags = (limit >> 16) & 0x0F;
    gdt[i].base_low = (base & 0xFFFF);
    gdt[i].base_mid = (base >> 16) & 0xFF;
    gdt[i].base_high = (base >> 24) & 0xFF;
    gdt[i].access_byte = access;
    gdt[i].flags |= gran & 0xF0;

    // Second 8 bytes (upper 32 bits of base address for 64-bit)
    gdt[i + 1].base_upper = (base >> 32) & 0xFFFFFFFF;
    gdt[i + 1].reserved = 0;
}

// TSS is used by the CPU for stack switching when privilege levels change
void init_tss()
{
    // Clear the entire TSS structure
    memset(&tss, 0, sizeof(tss_entry));

    // Set the kernel stack pointer (RSP0) for ring 0
    // When an interrupt or system call occurs from ring 3, CPU loads this stack
    // Stack grows downward, so we point to the end
    tss.rsp0 = (uint64_t)(kernel_stack + STACK_SIZE);

    // Set I/O permission bitmap offset beyond the TSS limit (no I/O bitmap)
    tss.iomap_offset = sizeof(tss_entry);

    // Entries 5-6: Task State Segment (selector 0x28)
    // TSS takes 16 bytes (2 GDT entries) in 64-bit mode
    // Access = 0x89: Present, Ring 0, Available TSS (type 9)
    // Binary: 10001001
    // Flags = 0x00: No special flags for TSS
    uint64_t tss_base = (uint64_t)&tss;
    uint32_t tss_limit = sizeof(tss_entry) - 1;
    tss_set_entry(5, tss_base, tss_limit, 0x89, 0x00);

    serial_printf("TSS initialized at: base=0x%p, rsp0=0x%p\n", (void*)&tss, (void*)tss.rsp0);
}
