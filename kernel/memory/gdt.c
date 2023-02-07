#include <stddef.h>
#include <stdint.h>

#include <memory/gdt.h>
#include <memory/memory.h>
#include <memory/pmm.h>

extern uint32_t stack_top;

struct tss {
    uint32_t back_link;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint32_t io_map_offset;
} __attribute__((aligned (128)));

// https://en.wikipedia.org/wiki/Global_Descriptor_Table
struct gdt_entry
{
    uint16_t segment_limit_1; // first 16 bits of the segment limit
    uint16_t base_1;          // first 16 bits of the base address
    uint8_t  base_2;          // bits 17-24 of the base address

    struct access_byte
    {
        // These flags are different between system and non-system segments
        uint8_t accessed : 1;   // set to 0, CPU will set if segement is accessed
        uint8_t r_w : 1;        // permissions for (code/data) segements (0 : no read/write, 1: read/write)
        uint8_t dc : 1;         // direction/conforming bit
        uint8_t e : 1;          // executable (0: data segement, 1: code segement)

        // The same for system and non-system segments
        uint8_t s : 1;          // descriptor type (0: system segment 1: code/data segment)
        uint8_t dpl : 2;        // descriptor privilege level (0: kernel, 3: userspace)
        uint8_t p : 1;          // present
    } __attribute__((packed)) access_byte;

    struct limit_and_flags // 8-bits
    {
        uint8_t segment_limit_2 : 4; // last 4 bits of the segment limit
        uint8_t res : 1;             // Reserved, set to zero
        uint8_t l : 1;               // Long-mode (0: 32-bit mode, 1: long mode)
        uint8_t db : 1;              // Size flags (0: 16-bit protected, 1: 32-bit protected)
        uint8_t g : 1;               // Granularity (0: 1 byte blocks, 1: 4KiB blocks)
    } __attribute__((packed)) limit_and_flags;

    uint8_t base_3;

} __attribute__((packed));

struct gdt_pointer
{
    uint16_t size;
    uint32_t addr;
} __attribute__((packed));

static struct gdt_entry   global_descriptor_table[64] = { 0 };
static struct gdt_pointer gdt_descriptor              = { 0 };

struct tss tss_region = { 0 };


static struct gdt_entry gdt_create_entry(uint32_t segment_limit, // 20-bit value
                                         uint32_t base,          // Segment base address
                                         uint8_t access_byte,    // Segment access rules
                                         uint8_t flags)          // Segment permissions
{
    struct gdt_entry gdt_entry = { 0 };

    gdt_entry.segment_limit_1 = (0xFFFF & segment_limit);
    gdt_entry.base_1          = (0x00FFFF & base);
    gdt_entry.base_2          = (0xFF0000 & base) >> 16;
    gdt_entry.access_byte     = *(struct access_byte *)&access_byte;

    uint8_t flags_and_limit   = ((segment_limit >> 16) & 0x0F) | ((flags << 4) & 0xF0);
    gdt_entry.limit_and_flags = *(struct limit_and_flags *)&flags_and_limit;

    gdt_entry.base_3 = (base & 0xFF000000) >> 24;

    return gdt_entry;
}

uint32_t gdt_init()
{
    // Entry 0 is never referenced by the processor and should be filled with 0
    global_descriptor_table[0] = gdt_create_entry(0, 0, 0, 0);  // NULL Segment

    global_descriptor_table[1] = gdt_create_entry(0xFFFFF, 0, 0b10011010, 0b1100); // Kernel Code Segment
    global_descriptor_table[2] = gdt_create_entry(0xFFFFF, 0, 0b10010010, 0b1100); // Kernel Data Segment

    global_descriptor_table[3] = gdt_create_entry(sizeof(tss_region), (uint32_t)&tss_region, 0b10001001, 0); // Task State Segment

    global_descriptor_table[4] = gdt_create_entry(0xFFFFF, 0, 0b11111010, 0b1100); // User Code Segment
    global_descriptor_table[5] = gdt_create_entry(0xFFFFF, 0, 0b11110010, 0b1100); // User Data Segment

    uint32_t num_gdt_entires = 6;

    gdt_descriptor.size = (num_gdt_entires * sizeof(struct gdt_entry));
    gdt_descriptor.addr = (uint32_t)&global_descriptor_table;

    tss_region.ss0  = GDT_KERNEL_DATA_SEG;

    return ((uint32_t)&gdt_descriptor);
}

void gdt_update_tss(uint32_t user_kernel_stack)
{
    tss_region.esp0 = user_kernel_stack;
}
