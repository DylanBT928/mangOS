#include "cpu/idt.h"

#include "drivers/serial.h"
#include "kernel.h"
#include "libc/string.h"

idtr idtr_ptr;
extern void load_idt();

void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t selector, uint8_t attributes)
{
    idt[vector].offset_low = handler & 0xFFFF;
    idt[vector].offset_mid = (handler >> 16) & 0xFFFF;
    idt[vector].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[vector].selector = selector;
    idt[vector].attributes = attributes;
    idt[vector].reserved = 0;
    idt[vector].ist = 0;
}

// lets the compiler know this is an interrupt handler
__attribute__((interrupt)) void isr_divide_by_zero(interrupt_frame* frame)
{
    serial_print("error: Division by zero\n");
    serial_printf("RIP = 0x%lx\n", frame->rip);

    hcf();
}

void init_idt(void)
{
    // Clear the IDT
    memset(&idt, 0, sizeof(idt_entry) * 256);

    // Set up the IDT Descriptor (IDTR)
    idtr_ptr.limit = (sizeof(idt_entry) * 256) - 1;
    idtr_ptr.base = (uint64_t)&idt;

    // Set up ISRs Here with idt_set_gate
    idt_set_gate(0, (uint64_t)isr_divide_by_zero, 0x08, IDT_ATTR_INTERRUPT);

    serial_printf("Loading IDT at: base=0x%p, limit=0x%x\n", (void*)idtr_ptr.base, idtr_ptr.limit);

    load_idt();

    serial_printf("Loaded IDT successfully.\n");
}

void test_divide_by_zero(void)
{
    volatile int x = 1;
    volatile int y = 0;
    volatile int z = x / y;
}
