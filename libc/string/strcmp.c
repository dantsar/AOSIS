#include <string.h>

int strcmp(const char *s1, const char *s2)
{
    size_t i;
    for (i = 0; s1[i] && s2[i]; i++) {
        if (s1[i] == s2[i]) {
            continue;
        }
        return s1[i] - s2[i];
    }
    return s1[i] - s2[i];
}