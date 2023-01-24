#pragma once

#include <stdbool.h>

struct shell_command {
    const char *name;
    const char *msg;
    bool (*cmd)();
};

void kshell();
