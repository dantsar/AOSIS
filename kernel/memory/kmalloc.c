#include <stddef.h>
#include <stdint.h>

#include <memory/pmm.h>
#include <memory/kmalloc.h>
#include <memory/vmm.h>

#define BLOCK_HEADER_SIZE   (8U)
#define SMALLEST_BLOCK_SIZE (32U) // smallest block size (including header)

// The following is an implementation of the K&R malloc. There are tons of resources about the K&R malloc strategy
// and the following is a good reference (https://www.cs.princeton.edu/courses/archive/fall12/cos217/lectures/21DynamicMemory2.pdf)
//
// However, the tl;dr of the memomry management strategy is that a linked list of the free memory is kept.
// Blocks (free memory portions) can be divided into smaller blocks when allocating and coalesced (merged)
// the freeing used memory portions.
//
// Each block has a header that describes the size of the memory, and a pointer to the next block, when
// memory is allocated, the header is not passed to the "user"

struct memory_block
{
    // header of the memory block (size 8)
    size_t size;                     // size includes the header
    struct memory_block *next_block; // next block in list

    uint8_t *free_memory; // ptr to the start of the free memory
};

struct memory_block *block_list_head = NULL;
struct memory_block *block_list_tail = NULL;

static uint8_t *kmalloc_split_block(struct memory_block *block, size_t alloc_size);
static void     kmalloc_coalesce(struct memory_block *prev_block, struct memory_block *new_block, struct memory_block *next_block);

// TESTING AND VALIDATING KMALLOC
uint32_t kmalloc_test[1024] __attribute__((aligned (4096))) = { 0 };


// if a block is too large, divide the block and allocate the second block
static uint8_t *kmalloc_split_block(struct memory_block *block, size_t alloc_size)
{
    struct memory_block *new_block = (struct memory_block *)((uint32_t)block + block->size - (alloc_size + BLOCK_HEADER_SIZE)); // still need to double, triple check this one

    new_block->size        = alloc_size + BLOCK_HEADER_SIZE;
    new_block->next_block  = NULL;
    new_block->free_memory = (uint8_t *)(BLOCK_HEADER_SIZE + (uint32_t)new_block); // I'm not 100% sure about the pointer math here

    block->size -= new_block->size;

    return new_block->free_memory;
}

static void kmalloc_coalesce(struct memory_block *prev_block,
                             struct memory_block *new_block,
                             struct memory_block *next_block)
{
    if (next_block != NULL)
    {
        if ((uint32_t)next_block == ((uint32_t)new_block + new_block->size))
        {
            new_block->next_block = next_block->next_block;

            new_block->size += next_block->size;

            // update the block list tail if the last block is being coalesced
            if (block_list_tail == next_block)
            {
                block_list_tail = new_block;
            }
        }

    }

    if (prev_block != NULL)
    {
        if ((uint32_t)new_block == ((uint32_t)prev_block + prev_block->size))
        {
            prev_block->next_block = new_block->next_block;

            prev_block->size += new_block->size;
        }
    }
}


void kmalloc_init(void)
{
    // set up the initial blocks
    block_list_head = (struct memory_block *)kmalloc_test;

    block_list_head->size        = sizeof(kmalloc_test);
    block_list_head->next_block  = NULL;
    block_list_head->free_memory = (uint8_t *)(BLOCK_HEADER_SIZE + (uint32_t)kmalloc_test);

    block_list_tail = block_list_head;
}

void *kmalloc(size_t size)
{
    if (size == 0U)
    {
        return NULL;
    }

    void *alloc_ptr   = NULL;
    size_t alloc_size = ((size + 0b111) >> 3) << 3; // round up size to the nearest 8 bytes

    // find block in the free list that has enough size, and split the block if it's too large
    struct memory_block *block = block_list_head;
    while (block != NULL)
    {
        uint32_t free_mem_in_block = block->size - BLOCK_HEADER_SIZE; // size of free memory

        if (alloc_size <= free_mem_in_block)
        {
            uint32_t mem_left_over = free_mem_in_block - alloc_size; // unused memory after allocation

            if (mem_left_over >= SMALLEST_BLOCK_SIZE)
            {
                // divide the free block into two and allocate the second block
                alloc_ptr = kmalloc_split_block(block, alloc_size);
            }
            else
            {
                alloc_ptr = block->free_memory;
            }

            break;
        }
        else
        {
            // block size not large enough
        }

        block = block->next_block;
    }

    // if a block was not allocated, increase the dynamic memory size
    if (alloc_ptr == NULL)
    {
        struct memory_block *new_free_block = (struct memory_block *)vmm_alloc_page();

        new_free_block->size        = PAGE_SIZE;
        new_free_block->free_memory = (uint8_t *)((uint32_t)new_free_block + BLOCK_HEADER_SIZE);
        new_free_block->next_block  = NULL;

        block_list_tail->next_block = new_free_block;
    }

    return alloc_ptr;
}

void kfree(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    // the general logic for freeing an element,
    // is that you increment the pointer to get the header
    // then check if you can coalesce the block to the right (next element in the free list) {pointer math}
    // then loop through the list to get the prev element, and check if you can coalesce to the left

    struct memory_block *new_block = (struct memory_block *)((uint32_t)ptr - BLOCK_HEADER_SIZE);

    // Necessary for coalesce, get the next and previous blocks of new_block

    struct memory_block *prev_block = NULL;
    struct memory_block *next_block = NULL;

    if (new_block < block_list_head) // edge case: new_block belongs at the beginning of the free list
    {
        next_block = block_list_head;
    }
    else
    {
        struct memory_block *block = block_list_head;
        while (block != NULL)
        {
            if ((block < new_block) && (block->next_block > new_block || block->next_block == NULL))
            {
                prev_block = block;
                next_block = prev_block->next_block; // potentially NULL

                break;
            }

            block = block->next_block;
        }
    }

    if (prev_block == NULL)
    {
        // Append to the beginning of the list
        new_block->next_block = next_block;
        block_list_head = new_block;
    }
    else if (next_block == NULL)
    {
        // Append to the end of the list
        prev_block->next_block = new_block;
        new_block->next_block  = NULL;
        block_list_tail        = new_block;
    }
    else
    {
        // Insert into the middle of the list
        prev_block->next_block = new_block;
        new_block->next_block  = next_block;
    }

    kmalloc_coalesce(prev_block, new_block, next_block);
}
