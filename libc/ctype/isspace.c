#include <ctype.h>

int isspace(int c)
{
    switch (c) {
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        case ' ':
            return 0;
    }

    return 1;
}
