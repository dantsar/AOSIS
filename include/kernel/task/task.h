#pragma once

#include <stdint.h>

enum task_state
{
    NEW = 0,
    READY,
    ACTIVE,
    DONE,
    STOPPED
};

struct task
{
    // The following struct elements have to maintain their order because of
    // the assembly code use for switching
    uint32_t kernel_stack_top;
    // uint32_t stack_top; at the moment, not a thing because only switching between kernel
    // also, don't switch address spaces
    // struct virtual_address_space *vas;

    // The following struct elements can be in any order
    uint32_t pid;
    uint32_t page_directory;    // should this be uint32_t?
    uint32_t kernel_stack_base; //


    uint32_t start_address;     // starting execution address

    struct task *next_task;
};

void task_init(void);

struct task *task_create(void);
