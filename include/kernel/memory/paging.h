#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <task/task.h>

#define PAGE_TABLE_INDICES (1024U)
#define PAGE_TABLE_RANGE   (PAGE_TABLE_INDICES * PAGE_SIZE)

#define PAGE_DIRECTORY_INDEX(addr) (((uint32_t)(addr) >> 22U) & 0x3FFU)
#define PAGE_TABLE_INDEX(addr)     (((uint32_t)(addr) >> 12U) & 0x3FFU)


// 32-bit page directory entry that references a page table
struct page_directory_entry
{
    uint32_t present : 1;           // If set, page is actually in physical memory
    uint32_t r_w : 1;               // Read-write permissions, set read/write, clear read-only
    uint32_t user : 1;              // User/supervisor. 0: usermode access are not allowed to the memory
    uint32_t write_through : 1;     // Page-level write-through
    uint32_t cache_disable : 1;     // Page-level cache disable
    uint32_t accessed : 1;          // Set if software has accessed the 4KB page
    uint32_t ignored : 1;           // Ignored bit
    uint32_t page_size : 1;         // Determines pages size, set to 0 if 4KB page
    uint32_t ignored_2 : 4;         // Ignored bits
    uint32_t phys_addr : 20;        // Physical address of the 4KB page
} __attribute__((packed));

// 32-bit page table entry that maps a 4KB page
struct page_table_entry
{
    uint32_t present : 1;            // If set, page is actually in physical memory
    uint32_t r_w : 1;                // Read/write permissions of page. if set, page is read/write
    uint32_t user : 1;               // User bit. if set, page is accessible by all, else if not set, only accessible by the supervisor
    uint32_t write_through : 1;      // Page-level write-through
    uint32_t cache_disable : 1;      // Page-level cache disable
    uint32_t accessed : 1;           // et if software has accessed the 4KB page
    uint32_t dirty : 1;              // et if software has written to the 4KB page
    uint32_t pat_support : 1;        // If set, PAT is supported, otherwise reserved
    uint32_t global_translation : 1; // If set, determines whether the translation is global
    uint32_t ignored : 3;            // Ignored bits
    uint32_t phys_addr : 20;         // Physical address of the 4KB page
} __attribute__((packed));

struct page_directory
{
    struct page_directory_entry entries[1024];
} __attribute__((aligned (4096)));

struct page_table
{
    struct page_table_entry entries[1024];
} __attribute__((aligned (4096)));


void *paging_init();

// Insert the page table into the page directory
void paging_add_page_table(void *pt_phys_addr, void *pt_virt_addr);

bool paging_is_virtual_page_present(void *page_table_ptr, void *virt_page);

// Map a virtual page to a physical page and add to page table
// Returns the physcial address
void *paging_map_virtual_page(void *page_table_ptr, void *virt_page);

void *paging_map_virtual_page_user(void *page_table_ptr, void *virt_page);

void paging_create_userspace(struct user_task_info *user_task);
