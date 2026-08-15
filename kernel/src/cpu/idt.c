#include "cpu/idt.h"

#include "drivers/serial.h"
#include "kernel.h"
#include "libc/string.h"

idtr idtr_ptr;
static idt_entry idt[256];

extern void load_idt(idtr* idtr_ptr);

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

__attribute__((interrupt)) void isr_invalid_opcode(interrupt_frame* frame)
{
    serial_print("error: Invalid Opcode\n");
    serial_printf("RIP = 0x%lx\n", frame->rip);

    hcf();
}

__attribute__((interrupt)) void isr_double_fault(interrupt_frame* frame, uint64_t error_code)
{
    serial_printf("error: Double Fault (error code: 0x%lx)\n", error_code);
    serial_printf("RIP = 0x%lx\n", frame->rip);

    hcf();
}

__attribute__((interrupt)) void isr_general_protection_fault(interrupt_frame* frame, uint64_t error_code)
{
    serial_printf("error: General Protection Fault (error code: 0x%lx)\n", error_code);
    serial_printf("RIP = 0x%lx\n", frame->rip);

    hcf();
}

__attribute__((interrupt)) void isr_page_fault(interrupt_frame* frame, uint64_t error_code)
{
    uint64_t cr2;
    __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));

    serial_printf("error: Page Fault (error code: 0x%lx) accessing 0x%lx\n", error_code, cr2);
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
    idt_set_gate(6, (uint64_t)isr_invalid_opcode, 0x08, IDT_ATTR_INTERRUPT);
    idt_set_gate(8, (uint64_t)isr_double_fault, 0x08, IDT_ATTR_INTERRUPT);
    idt_set_gate(13, (uint64_t)isr_general_protection_fault, 0x08, IDT_ATTR_INTERRUPT);
    idt_set_gate(14, (uint64_t)isr_page_fault, 0x08, IDT_ATTR_INTERRUPT);

    serial_printf("Loading IDT at: base=0x%p, limit=0x%x\n", (void*)idtr_ptr.base, idtr_ptr.limit);

    load_idt(&idtr_ptr);

    serial_printf("Loaded IDT successfully.\n");
}

void test_divide_by_zero(void)
{
    volatile int x = 1;
    volatile int y = 0;
    volatile int z = x / y;
}
