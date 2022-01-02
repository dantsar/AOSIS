#include <ctype.h>

int isupper(int c)
{
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    return 0;
}