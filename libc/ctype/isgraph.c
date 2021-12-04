#include <ctype.h>

int isgraph(int c)
{
    if (c == ' ') {
        return 1;
    }

    return isprint(c);
}