[bits 64]
global load_gdt
global load_tss

load_gdt:
    ; load the GDT register
    lgdt [rdi]

    ; reload data segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; reload code segment
    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    db 0x48
    retf

load_tss:
    mov ax, 0x28
    ltr ax
    ret
