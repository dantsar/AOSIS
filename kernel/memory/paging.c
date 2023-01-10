#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <common/common_macros.h>
#include <interrupt/interrupt.h>
#include <kernel.h>
#include <memory/memory.h>
#include <memory/paging.h>
#include <memory/pmm.h>
#include <terminal/tty.h>

#define PAGE_DIRECTORY_INDEX(addr) (((addr) >> 22U) & 0x3FFU)
#define PAGE_TABLE_INDEX(addr)     (((addr) >> 12U) & 0x3FFU)

extern uint32_t kernel_start_addr_virt;

// 32-bit page directory entry that references a page table
struct page_directory_entry
{
    uint32_t present : 1;           // if set, page is actually in physical memory
    uint32_t r_w : 1;               // read-write permissions, set read/write, clear read-only
    uint32_t user : 1;              // user/supervisor. set: user, clear: supervisor
    uint32_t write_through : 1;     // page-level write-through
    uint32_t cache_disable : 1;     // page-level cache disable
    uint32_t accessed : 1;          // set if software has accessed the 4KB page
    uint32_t ignored : 1;
    uint32_t page_size : 1;
    uint32_t global : 1;
    uint32_t ignored_2 : 3;
    uint32_t phys_addr : 20;
};

// 32-bit page table entry that maps a 4KB page
struct page_table_entry
{
    uint32_t present : 1;            // if set, page is actually in physical memory
    uint32_t r_w : 1;                // read/write permissions of page. if set, page is read/write
    uint32_t user : 1;               // user bit. if set, page is accessible by all, else if not set, only accessible by the supervisor
    uint32_t write_through : 1;      // page-level write-through
    uint32_t cache_disable : 1;      // page-level cache disable
    uint32_t accessed : 1;           // set if software has accessed the 4KB page
    uint32_t dirty : 1;              // set if software has written to the 4KB page
    uint32_t pat_support : 1;        // if set, PAT is supported, otherwise reserved
    uint32_t global_translation : 1; // if set, determines whether the translation is global
    uint32_t ignored : 3;            // Ignored bits
    uint32_t phys_addr : 20;      // Physical address of the 4KB page
};

struct __attribute__((aligned (4096))) page_directory
{
    struct page_directory_entry entries[1024];
};

struct __attribute__((aligned (4096))) page_table
{
    struct page_table_entry entries[1024];
};

struct page_directory *kernel_page_directory;



// addr MUST be 4KB aligned
static void paging_set_page_table_addr(struct page_table_entry *entry, uint32_t addr)
{
    entry->phys_addr = (addr >> 12U);
}

// addr MUST be 4KB aligned
static void paging_set_page_directory_addr(struct page_directory_entry *entry, uint32_t addr)
{
    entry->phys_addr = (addr >> 12U);
}

static void paging_page_fault_handler(registers_t regs)
{
    UNUSED(regs);
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    kprintf("page fault occurred\n");
    kprintf("faulting address: %x\n", faulting_address);
    panic("page fault occurred\n");
}

void paging_init()
{
    // allocate a page directory
    kernel_page_directory = (struct page_directory *)P2V_ADDR(pmm_alloc_page());

    if (kernel_page_directory == NULL)
    {
        panic("Unable to allocate page directory\n");
    }
    else
    {
        memset(kernel_page_directory, 0, PAGE_SIZE);
    }

    // allocate a page table
    struct page_table *page_table = (struct page_table *)P2V_ADDR(pmm_alloc_page());

    if (page_table == NULL)
    {
        panic("Unable to allocate a kernel page table\n");
    }
    else
    {
        memset(page_table, 0, PAGE_SIZE);
    }

    // map the first 4MiB to virtual memory, including the first MiB because of the VGA bugger is below 1 MiB
    uint32_t i;
    uint32_t kernel_frame;
    for (kernel_frame = 0x00000000U, i = 0U;
         i < 1024U;
         kernel_frame += PAGE_SIZE, i++)
    {
        // create page table entry that maps the kernel
        struct page_table_entry entry = { 0 };
        paging_set_page_table_addr(&entry, PAGE_ALIGN(kernel_frame));
        entry.present = true;
        entry.r_w     = true;

        // map the kernel frames to the page table
        uint32_t page_table_index = PAGE_TABLE_INDEX(kernel_frame);
        page_table->entries[page_table_index] = entry;
    }

    // add the mapped page table to the page directory
    struct page_directory_entry page_directory_entry = { 0 };
    paging_set_page_directory_addr(&page_directory_entry, V2P_ADDR((uint32_t)&page_table->entries));
    page_directory_entry.present = true;
    page_directory_entry.r_w     = true;

    // add page table to the page directory
    uint32_t page_table_address                        = PAGE_DIRECTORY_INDEX((uint32_t)&kernel_start_addr_virt);
    kernel_page_directory->entries[page_table_address] = page_directory_entry;

    // Configure the page fault handler
    idt_handlers[14] = paging_page_fault_handler;

    uint32_t kernel_page_directory_addr = V2P_ADDR((uint32_t)(&kernel_page_directory->entries));

    // load page table into cr3
    asm volatile("mov %0, %%cr3":: "r"(kernel_page_directory_addr));

    // enable paging by setting bit 31 in cr0
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}
