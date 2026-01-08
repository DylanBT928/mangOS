#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Structure of a single (64-bit) IDT entry
typedef struct
{
    uint16_t offset_low;   // Lower 16 bits of ISR address
    uint16_t selector;     // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t ist;           // The IST in the TSS that the CPU will load into RSP; set to zero for now
    uint8_t attributes;    // Type and attributes
    uint8_t flags;         // Flags
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
static idtr idtr_ptr;

// IDT Flag Enumurations, based on https://wiki.osdev.org/Interrupt_Descriptor_Table
typedef struct
{
    IDT_FLAG_GATE_TASK = 0x5,
    IDT_FLAG_GATE_16BIT_INT = 0xE,       // Interupt Gate, used to specify Interupt Service Routine
        IDT_FLAG_GATE_16BIT_TRAP = 0xF,  // Trap Gate, used to deal with Exceptions, saves next instruction and retries

        IDT_FLAG_GATE_64BIT_INT = 0x8E,   // Same as 32bit but 64bit
        IDT_FLAG_GATE_64BIT_TRAP = 0x8F,  // Same as 32bit but 64bit

        IDT_FLAG_RING0 = (0 << 5),  // Descriptor Privledge Level, indicates maximum ring from which a software
        IDT_FLAG_RING1 = (1 << 5),  // interrupt may be called, does not apply to hardware interrupts
        IDT_FLAG_RING2 = (2 << 5),
    IDT_FLAG_RING3 = (3 << 5),

    IDT_FLAG_PRESENT = 0x80,  // 0: means interrupt descriptor is disabled, 1: interrupt descriptor is enabled

} IDT_FLAGS;

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
void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t selector, uint8_t flags, uint8_t attributes);
extern void load_idt(idtr* idtr_ptr);

// There are many more errors we can handle, such as
// page faults and overflows, but for simplicity, we'll handle division by error
// to ensure our IDT and ISRs are set up correctly
void isr_divide_by_zero(struct interrupt_frame* frame);

#endif
