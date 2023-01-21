#include <stddef.h>
#include <stdint.h>

#include <memory/gdt.h>
#include <memory/memory.h>

// https://en.wikipedia.org/wiki/Global_Descriptor_Table

struct gdt_entry
{
    uint16_t segment_limit_1; // first 16 bits of the segment limit
    uint16_t base_1;          // first 16 bits of the base address
    uint8_t  base_2;          // bits 17-24 of the base address

    struct access_byte // TODO: improve comments for the fields maybe create a union for regular and system segment descriptors
    {
        uint8_t accessed : 1;   // set to 0, CPU will set if segement is accessed
        uint8_t r_w : 1;        // permissions for (code/data) segements (0 : no read/write, 1: read/write)
        uint8_t dc : 1;         // direction/conforming bit
        uint8_t e : 1;          // executable (0: data segement, 1: code segement)
        uint8_t s : 1;          // descriptor type (0: system segment 1: code/data segment)
        uint8_t dpl : 2;        // descriptor privilege level
        uint8_t p : 1;          // present
    } __attribute__((packed)) access_byte;

    _Static_assert(sizeof(struct access_byte)==1,"struct access_byte not packed");

    struct limit_and_flags // 8-bits
    {
        uint8_t segment_limit_2 : 4; // last 4 bits of the segment limit
        uint8_t res : 1;             // Reserved
        uint8_t l : 1;               // Long-mode (0: )
        uint8_t db : 1;              // Size flags (0: 16-bit protected, 1: 32-bit protected)
        uint8_t g : 1;               // Granularity (0: 1 byte blocks, 1: 4KiB blocks)
    } __attribute__((packed)) limit_and_flags;

    _Static_assert(sizeof(struct limit_and_flags)==1,"struct limit_and_flags not packed");

    uint8_t base_3;

} __attribute__((packed));

_Static_assert(sizeof(struct gdt_entry)==8,"struct gdt_entry not packed");


struct gdt_pointer
{
    uint16_t size;
    uint32_t addr;
} __attribute__((packed));

static struct gdt_entry global_descriptor_table[64] = { 0 };
static struct gdt_pointer gdt_descriptor            = { 0 };

static uint8_t tss_region[256];

// TODO: clean this up and add comments
static struct gdt_entry gdt_create_entry(uint32_t segment_limit, // 20-bit value
                                         uint32_t base,
                                         uint8_t access_byte,
                                         uint8_t flags)
{
    struct gdt_entry gdt_entry = { 0 };

    gdt_entry.segment_limit_1 = (0xFFFF & segment_limit);
    gdt_entry.base_1          = (0x00FFFF & base);
    gdt_entry.base_2          = (0xFF0000 & base) >> 16;
    gdt_entry.access_byte     = *(struct access_byte *)&access_byte;

    flags                     = ((segment_limit >> 16) & 0x0F) | ((flags << 4) & 0xF0);
    gdt_entry.limit_and_flags = *(struct limit_and_flags *)&flags;

    gdt_entry.base_3          = (base & 0xFF000000) >> 24;

    return gdt_entry;
}

uint32_t gdt_init()
{
    // Entry 0 is NEVER referenced by the processor and should be filled with 0
    global_descriptor_table[0] = gdt_create_entry(0, 0, 0, 0);  // NULL Segment

    global_descriptor_table[1] = gdt_create_entry(0xFFFFF, 0, 0b10011010, 0b1100); // Code Segment
    global_descriptor_table[2] = gdt_create_entry(0xFFFFF, 0, 0b10010010, 0b1100); // Data Segment

    // TODO: update this (currently using values from https://wiki.osdev.org/GDT_Tutorial)
    global_descriptor_table[3] = gdt_create_entry(sizeof(tss_region), (uint32_t)&tss_region, 0x89, 0); // Task State Segment

    uint32_t num_gdt_entires = 4;

    gdt_descriptor.size = (num_gdt_entires * sizeof(struct gdt_entry));
    gdt_descriptor.addr = (uint32_t)&global_descriptor_table;

    return ((uint32_t)&gdt_descriptor);
}
