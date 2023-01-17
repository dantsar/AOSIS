#pragma once

#include <stdint.h>

struct task
{
    uint32_t pid;

    uint32_t kernel_stack_top;
    uint32_t page_directory;

    // next task
    // state
};

void task_init(void);

