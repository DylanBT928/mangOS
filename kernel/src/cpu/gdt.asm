[bits 64]
global load_gdt
global load_tss

load_gdt:
    ; load the GDT register
    lgdt [rdi]

reload_cs:
    ; reload code segment
    push 0x08
    lea rax, [rel .reload_ds]
    push rax
    retfq

.reload_ds:
    ; reload data segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

load_tss:
    mov ax, 0x28
    ltr ax
    ret
