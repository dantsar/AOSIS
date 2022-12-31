#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <interrupt/interrupt.h>
#include <kernel.h>
#include <memory/paging.h>
#include <memory/pmm.h>
#include <terminal/tty.h>

#define PAGE_DIRECTORY_INDEX(addr) (((addr) >> 22) & 0x3FFU)
#define PAGE_TABLE_INDEX(addr)     (((addr) >> 12) & 0x3FFU)

extern uint32_t kernel_start_address;
extern uint32_t kernel_end_address;

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

// page directory
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


static struct page_table other_page_table = { 0 };

static void paging_page_fault_handler(registers_t regs)
{
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    kprintf("page fault occurred\n");

    kprintf("faulting address: %x\n", faulting_address);

    // create a new entry
    // create a new page table
    // do some checks
        // check if the page directory exists
        // check if the page table exists ??? otherwise it's a permissison thing, but for now, I'll assume that only the page table doens't exist

    // hardcode it for now

    // add the page table entry to this guy
    struct page_table *new_page_table = &other_page_table;

    struct page_table_entry pte;
    paging_set_page_table_addr(&pte, (uint32_t)pmm_alloc_page());
    pte.accessed = 1U;
    new_page_table->entries[PAGE_TABLE_INDEX(faulting_address)] = pte;

    struct page_directory_entry pde;
    paging_set_page_directory_addr(&pde, PAGE_ALIGN((uint32_t)&pte));
    pde.accessed = 1U;

    kernel_page_directory->entries[PAGE_DIRECTORY_INDEX(faulting_address)] = pde;

    panic("page fault occurred\n");
}

void paging_init()
{
    // allocate a page directory
    kernel_page_directory = (struct page_directory *) pmm_alloc_page();

    if (kernel_page_directory == NULL)
    {
        panic("Unable to allocate page directory\n");
    }
    else
    {
        memset(kernel_page_directory, 0, PAGE_SIZE);
    }

    // allocate a page table
    struct page_table *page_table = (struct page_table *)pmm_alloc_page();

    if (page_table == NULL)
    {
        panic("Unable to allocate a kernel page table\n");
    }
    else
    {
        memset(page_table, 0, PAGE_SIZE);
    }

    // go through a map the kernel to the page table (not doing a higher half kernel ATM, so kernel address is 1:1 with physical and virtual)
    for (uint8_t *kernel_frame = NULL;// (uint8_t *)&kernel_start_address; // map in the first megabyte inorder to use vga buffer
         kernel_frame < (uint8_t *)&kernel_end_address;
         kernel_frame += PAGE_SIZE)
    {
        // create page table entry that maps the kernel
        struct page_table_entry entry = { 0 };
        paging_set_page_table_addr(&entry, PAGE_ALIGN((uint32_t)kernel_frame));
        entry.present = 1;

        // map the kernel frames to the page table
        uint32_t page_table_index = PAGE_TABLE_INDEX((uint32_t)kernel_frame);
        page_table->entries[page_table_index] = entry;
    }

    // add the mapped page table to the page directory
    struct page_directory_entry page_directory_entry = { 0 };
    paging_set_page_directory_addr(&page_directory_entry, (uint32_t)page_table->entries);
    page_directory_entry.present = 1;

    uint32_t page_table_address = PAGE_DIRECTORY_INDEX((uint32_t)&kernel_start_address);
    kernel_page_directory->entries[page_table_address] = page_directory_entry;

    // Configure the page fault handler
    idt_handlers[14] = paging_page_fault_handler;

    // load page table into cr3
    asm volatile("mov %0, %%cr3":: "r"(kernel_page_directory));

    // enable paging by setting bit 31 in cr0
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));


    /*
        test page fault
    */
    uint8_t *test = 0xC0000000;
    *test = 0;
    /*
        test page fault
    */
}
