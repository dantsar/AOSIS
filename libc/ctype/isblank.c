#include <ctype.h>

int isblank(int c)
{
    if (c == ' ' || c == '\t') {
        return 0;
    }
    return 1;
}