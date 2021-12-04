#include <ctype.h>

int isascii(int c) {
    if (c <= 127) {
        return 0;
    }
    return 1;
}
