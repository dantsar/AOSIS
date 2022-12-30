#pragma once

#include <stdint.h>

// page entry
struct page_entry
{
    uint32_t present : 1;       // if set, page is actually in physical memory
    uint32_t r_w : 1;           // read/write permissions of page. if set, page is read/write
    uint32_t user : 1;          // user bit. if set, page is accessible by all, else if not set, only accessible by the supervisor
    uint32_t write_through : 1; //
    uint32_t cache_disable : 1; //
    uint32_t accessed : 1;      //
    uint32_t dirty : 1;         //

    uint32_t page_size;
    uint32_t global : 1;
    uint32_t pat : 1;

    // uint32_t ;
    // uint32_t ;

    uint32_t frame : 20;
};

struct page_table
{
    struct page_entry page_table[1024];
};

// page directory
struct page_directory
{
    struct page_table *page_table[1024];



    uint32_t base;  // base address of page directory
};

