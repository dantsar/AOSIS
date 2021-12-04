#include <ctype.h>

int iscntrl(int c)
{
    if (c <= 31 || c == 127) {
        return 0;
    }

    return 1;
}