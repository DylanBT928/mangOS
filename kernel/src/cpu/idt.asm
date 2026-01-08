; loads the idt into the processor

global load_idt
extern idtr
load_idt:
    lidt [idtr]
    ret
