#include <ctype.h>

int ispunct(int c)
{
    if (c >=  33 && c <=  47 || 
        c >=  58 && c <=  64 || 
        c >= 123 && c <= 126) {
        return 0;
    }
    return 1;
}
