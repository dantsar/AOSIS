#include <string.h>

char *strcat(char *dest, const char *src)
{
    size_t i = 0;
    while (dest[i]) {
        i++;
    }

    size_t j = 0;
    do {
        dest[i + j] = src[j];
    }while (src[j++]);
    
    return dest;
}