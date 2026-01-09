; loads the idt into the processor
[bits 64]

global load_idt
extern idtr_ptr

load_idt:
    lidt [idtr_ptr]
    ret