#include "idt.h"


void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t selector, uint8_t flags, uint8_t attributes) {
    idt[vector].offset_low = handler & 0xFFFF;
    idt[vector].offset_mid = (handler >> 16) & 0xFFFF;
    idt[vector].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[vector].selector = selector;
    idt[vector].flags = flags;
    idt[vector].attributes = attributes;
    idt[vector].reserved = 0;
}

void isr_divide_by_zero() {};

void init_idt(void) {
    // Clear the IDT
    memset(&idt, 0, sizeof(idt_entry) * 256);

    // Set up the IDT Descriptor (IDTR)
    idtr.limit = (sizeof(idt_entry) * 256) - 1;
    idtr.base = (uint64_t)&idt;

    // Set up ISRs Here with idt_set_gate
    idt_set_gate(0, i686_ISR0, 0x08, IDT_FLAG_RING0 | IDT_FLAG_GATE_64BIT_INT)

    serial_printf("Loading IDT at: base=0x%p, limit=0x%x\n", (void*)idtr_ptr.base, idtr_ptr.limit);

    load_idt(idtr_ptr);

    serial_printf("Loaded IDT successfully.");
}


