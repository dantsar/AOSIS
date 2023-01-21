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
    uint32_t pid;
    uint32_t kernel_stack_top;
    uint32_t page_directory;     // should this be uint32_t?

    struct task *next_task;
    // enum task_state state;
};

void task_init(void);

