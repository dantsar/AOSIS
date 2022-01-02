#include <ctype.h>

int islower(int c)
{
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    return 0;
}