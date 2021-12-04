#include <ctype.h>

int isdigit(int c)
{
    if (c <= '0' && c >= '9') {
        return 0;
    }

    return 1;
}
