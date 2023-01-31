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
#include <memory/vmm.h>
#include <terminal/tty.h>

extern uint32_t kernel_start_addr_virt;

struct page_directory *kernel_page_directory; // TODO: make this static


static void paging_set_page_directory_addr(struct page_directory_entry *entry, uint32_t addr);

static void paging_flush_tlb();

static void paging_page_fault_handler(struct idt_registers regs);

static void paging_set_page_directory_addr(struct page_directory_entry *entry, uint32_t addr)
{
    entry->phys_addr = (addr >> 12U);
}

static void paging_set_page_table_addr(struct page_table_entry *entry, uint32_t addr)
{
    entry->phys_addr = (addr >> 12U);
}

static void paging_flush_tlb()
{
    asm volatile("movl %cr3,%eax");
    asm volatile("movl %eax, %cr3");
}

static void paging_page_fault_handler(struct idt_registers regs)
{
    UNUSED(regs);
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    kprintf("page fault occurred\n");
    kprintf("faulting address: %x\n", faulting_address);
    panic("page fault occurred\n");
}

void *paging_init()
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
    struct page_directory_entry pd_entry = { 0 };
    paging_set_page_directory_addr(&pd_entry, V2P_ADDR((uint32_t)&page_table->entries));
    pd_entry.present = true;
    pd_entry.r_w     = true;

    // add page table to the page directory
    uint32_t page_table_address                        = PAGE_DIRECTORY_INDEX(&kernel_start_addr_virt);
    kernel_page_directory->entries[page_table_address] = pd_entry;

    /* **************  TODO: delete this kludge **************/
    // // testing userspace here
    struct page_table *userspace_page_table = (struct page_table *)P2V_ADDR(pmm_alloc_page());
    for (uint32_t i = 0; i < 1024; i++)
    {
        userspace_page_table->entries[i]      = page_table->entries[i];
        userspace_page_table->entries[i].user = true;
    }

    // add the mapped page table to the page directory

    paging_set_page_directory_addr(&pd_entry, V2P_ADDR((uint32_t)&userspace_page_table->entries));
    pd_entry.present = true;
    pd_entry.r_w     = true;
    pd_entry.user    = true;

    // add page table to the page directory
    kernel_page_directory->entries[0] = pd_entry;
    /* ************** done testing userspace **************/

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

    paging_flush_tlb();

    return (void *)page_table; // return the virtual address of the initial page table for vmm_init
}

void paging_add_page_table(void *pt_phys_addr, void *pt_virt_addr)
{
    struct page_directory_entry pd_entry = { 0 };

    paging_set_page_directory_addr(&pd_entry, (uint32_t)pt_phys_addr);
    pd_entry.present = true;
    pd_entry.r_w     = true;

    uint32_t page_table_address                        = PAGE_DIRECTORY_INDEX(pt_virt_addr);
    kernel_page_directory->entries[page_table_address] = pd_entry;

    paging_flush_tlb();
}

bool paging_is_virtual_page_present(void *page_table_ptr, void *virt_page)
{
    // CHECK: is virt_page in page_table
    uint32_t pt_index = PAGE_TABLE_INDEX((uint32_t)virt_page);

    struct page_table *page_table     = (struct page_table *)page_table_ptr;
    struct page_table_entry *pt_entry = &(page_table->entries[pt_index]);

    return pt_entry->present;
}

void *paging_map_virtual_page(void *page_table_ptr, void *virt_page)
{
    uint32_t pt_index = PAGE_TABLE_INDEX((uint32_t)virt_page);

    struct page_table *page_table     = (struct page_table *)page_table_ptr;
    struct page_table_entry *pt_entry = &(page_table->entries[pt_index]);

    void *phys_page = pmm_alloc_page();
    paging_set_page_table_addr(pt_entry, (uint32_t)phys_page);

    pt_entry->present = true;
    pt_entry->r_w     = true;

    return phys_page;
}

// TODO: delete this
void *paging_map_virtual_page_user(void *page_table_ptr, void *virt_page)
{
    uint32_t pt_index = PAGE_TABLE_INDEX((uint32_t)virt_page);

    struct page_table *page_table     = (struct page_table *)page_table_ptr;
    struct page_table_entry *pt_entry = &(page_table->entries[pt_index]);

    void *phys_page = pmm_alloc_page();
    paging_set_page_table_addr(pt_entry, (uint32_t)phys_page);

    pt_entry->present = true;
    pt_entry->r_w     = true;
    pt_entry->user    = true;

    paging_flush_tlb();

    return phys_page;
}

// I need to copy/link the kernel address space into the new page directory
uint32_t paging_create_userspace(struct user_task_info *user_task)
{
    struct page_directory *new_userspace = (struct page_directory *)vmm_alloc_page();
    uint32_t user_page_directory_addr = vmm_get_phys_addr((void *)new_userspace);

    // copy the current kernel page directory
    for (uint32_t i = PAGE_DIRECTORY_INDEX(KERNEL_VIRT_BASE); i < PAGE_TABLE_INDICES; i++)
    {
        new_userspace->entries[i] = kernel_page_directory->entries[i];
    }

    /////////////////////////////////////////////////////////////////////////////////////

    // create a new page &&/|| page table for the actual user program
    struct page_table *user_program_pt = (struct page_table *)vmm_alloc_page();
    uint32_t user_prog_pt_addr         = vmm_get_phys_addr((void *)user_program_pt);

    struct page_table_entry prog_pt_entry = {
        .present   = true,
        .r_w       = true,
        .user      = true,
    };
    paging_set_page_table_addr(&prog_pt_entry, user_task->phys_addr_start);

    prog_pt_entry.phys_addr = user_task->phys_addr_start;

    // User program believes that it's loaded at address = 0
    user_program_pt->entries[0] = prog_pt_entry;

    struct page_directory_entry prog_pd_entry = {
        .present = true,
        .r_w     = true,
        .user    = true,
    };
    paging_set_page_directory_addr(&prog_pd_entry, user_prog_pt_addr);

    // User program believes that it's loaded at address = 0
    new_userspace->entries[0] = prog_pd_entry;

    /////////////////////////////////////////////////////////////////////////////////////

    // Allocate memory for the stack
    uint32_t stack_address              = (KERNEL_VIRT_BASE - PAGE_SIZE);
    uint32_t stack_page_directory_index = PAGE_DIRECTORY_INDEX(stack_address);
    uint32_t stack_page_table_index     = PAGE_TABLE_INDEX(stack_address);

    struct page_table *user_stack_pt = (struct page_table *)vmm_alloc_page();
    uint32_t user_stack_pt_addr      = vmm_get_phys_addr((void *)user_stack_pt);

    struct page_table_entry stack_pt_entry = {
        .present   = true,
        .r_w       = true,
        .user      = true,
    };
    paging_set_page_table_addr(&stack_pt_entry, user_task->phys_addr_start);

    stack_pt_entry.phys_addr = user_task->phys_addr_start;

    // User program believes that it's loaded at address = 0
    user_stack_pt->entries[stack_page_table_index] = stack_pt_entry;

    struct page_directory_entry stack_pd_entry = {
        .present = true,
        .r_w     = true,
        .user    = true,
    };
    paging_set_page_directory_addr(&stack_pd_entry, user_stack_pt_addr);

    // stack will be place at the very top of the user's memory
    new_userspace->entries[stack_page_directory_index] = stack_pd_entry;

    /////////////////////////////////////////////////////////////////////////////////////

    return user_page_directory_addr;
}
