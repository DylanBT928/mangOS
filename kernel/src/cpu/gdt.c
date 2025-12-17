#include "cpu/gdt.h"
#include "drivers/serial.h"

#define GDT_ENTRIES 7

gdt_entry_tss gdt[GDT_ENTRIES];

void gdt_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    gdt[i].limit = limit & 0xFFFF;              
    gdt[i].flags = (limit >> 16) & 0x0F;        

    gdt[i].base_low = base & 0xFFFF;            
    gdt[i].base_mid = (base >> 16) & 0xFF;     
    gdt[i].base_high = (base >> 24) & 0xFF;    

    gdt[i].access_byte = access;

    gdt[i].flags |= (flags & 0xF0);
}

void init_gdt() 
{
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xA0);
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xA0);
    gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xA0);
    gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xA0);
}