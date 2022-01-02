#ifndef _SHELL_H
#define _SHELL_H

#include <stdbool.h>

struct shell_command {
    const char *name;
    const char *msg;
    bool (*cmd)();
};

void kconsole();

#endif
