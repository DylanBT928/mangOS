#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT flags, based on https://wiki.osdev.org/Interrupt_Descriptor_Table
#define IDT_ATTR_PRESENT (1 << 7)           
#define IDT_ATTR_DPL_RING0 (0 << 5)         
#define IDT_ATTR_GATE_INTERRUPT 0x0E        
#define IDT_ATTR_INTERRUPT (IDT_ATTR_PRESENT | IDT_ATTR_DPL_RING0 | IDT_ATTR_GATE_INTERRUPT)

// Structure of a single (64-bit) IDT entry
typedef struct
{
    uint16_t offset_low;   // Lower 16 bits of ISR address
    uint16_t selector;     // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t ist;           // The IST in the TSS that the CPU will load into RSP; set to zero for now
    uint8_t attributes;    // Type and attributes
    uint16_t offset_mid;   // Middle 16 bits of ISR address
    uint32_t offset_high;  // Upper 32 bits of ISR address
    uint32_t reserved;     // Must be zero
} __attribute__((packed)) idt_entry;

// IDT Descriptor (IDTR): what gets loaded into the CPU
typedef struct
{
    uint16_t limit;  // Size of IDT - 1
    uint64_t base;   // Base address of IDT
} __attribute__((packed)) idtr;

static idt_entry idt[256];
extern idtr idtr_ptr;

// Interrupt frame pushed by CPU during interrupt
// Reference: https://wiki.osdev.org/Interrupt_Service_Routines
typedef struct
{
    uint64_t rip;       // address of the instruction that caused the interrupt/exception
    uint64_t cs;        // code segment selector active when the interrupt occurred
    uint64_t rflags;    // CPU flags register (interrupt flag, zero flag, carry flag, etc.)
    uint64_t rsp;       // stack pointer value before the CPU switched stacks
    uint64_t ss;        // stack segment selector active before the interrupt
} __attribute__((packed)) interrupt_frame;


void init_idt(void);
void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t selector, uint8_t attributes);

// There are many more errors we can handle, such as
// page faults and overflows, but for simplicity, we'll handle division by error
// to ensure our IDT and ISRs are set up correctly
void isr_divide_by_zero(interrupt_frame *frame);
void test_divide_by_zero(void);

#endif