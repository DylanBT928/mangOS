; loads the idt into the processor
[bits 64]

global load_idt

load_idt:
    lidt [rdi]
    ret

