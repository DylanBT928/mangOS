#ifndef IDT_H
#define IDT_H

#include <stdint.h> 

// Structure of a single (64-bit) IDT entry
typedef struct
{
    uint16_t offset_low;    // Lower 16 bits of ISR address
    uint16_t selector;      // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t ist;            // The IST in the TSS that the CPU will load into RSP; set to zero for now
    uint8_t attributes;     // Type and attributes
    uint16_t offset_mid;    // Middle 16 bits of ISR address
    uint32_t offset_high;   // Upper 32 bits of ISR address
    uint32_t reserved;      // Must be zero
} __attribute__((packed)) idt_entry;

// IDT Descriptor (IDTR): what gets loaded into the CPU
typedef struct
{
    uint16_t limit;         // Size of IDT - 1
    uint64_t base;          // Base address of IDT
} __attribute__((packed)) idtr;

static idt_entry idt[256];
static idtr idtr_ptr;

// Interrupt frame pushed by CPU during interrupt
// Reference: https://wiki.osdev.org/Interrupt_Service_Routines
struct
{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} interrupt_frame;  

void init_idt(void);
void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t selector, uint8_t attributes);

// There are many more errors we can handle, such as 
// page faults and overflows, but for simplicity, we'll handle division by error
// to ensure our IDT and ISRs are set up correctly
void isr_divide_by_zero(struct interrupt_frame *frame);

#endif