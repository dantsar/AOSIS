#pragma once

#include <stdint.h>

#define PAGE_DIRECTORY_INDEX(addr) (((addr) >> 22U) & 0x3FFU)
#define PAGE_TABLE_INDEX(addr)     (((addr) >> 12U) & 0x3FFU)

// 32-bit page directory entry that references a page table
struct page_directory_entry
{
    uint32_t present : 1;           // If set, page is actually in physical memory
    uint32_t r_w : 1;               // Read-write permissions, set read/write, clear read-only
    uint32_t user : 1;              // User/supervisor. set: user, clear: supervisor
    uint32_t write_through : 1;     // Page-level write-through
    uint32_t cache_disable : 1;     // Page-level cache disable
    uint32_t accessed : 1;          // Set if software has accessed the 4KB page
    uint32_t ignored : 1;           // Ignored bit
    uint32_t page_size : 1;         // Determines pages size, set to 0 if 4KB page
    uint32_t ignored_2 : 4;         // Ignored bits
    uint32_t phys_addr : 20;        // Physical address of the 4KB page
};

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
};

struct __attribute__((aligned (4096))) page_directory
{
    struct page_directory_entry entries[1024];
};

struct __attribute__((aligned (4096))) page_table
{
    struct page_table_entry entries[1024];
};

// TODO: clean this up, maybe make paging.c provide an interface for interacting with page tables
//       maybe a useful abstraction for vmm.c <...>

uint8_t *paging_init();

void paging_set_page_table_addr(struct page_table_entry *entry, uint32_t addr);

void paging_add_page_table(uint8_t *pt_phys_addr, uint8_t *pt_virt_addr);
