#include "json.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char* chtoa(char* dest, char ch, size_t* remLen) {
    if (*remLen > 0) {
        *dest++ = ch;
        *dest = '\0';
        (*remLen)--;
    }
    return dest;
}

static char* atoa(char* dest, const char* src, size_t* remLen) {
    while (*src && *remLen > 0) {
        *dest++ = *src++;
        (*remLen)--;
    }
    *dest = '\0';
    return dest;
}

char* json_objOpen(char* dest, const char* name, size_t* remLen) {
    if (name) {
        dest = chtoa(dest, '\"', remLen);
        dest = atoa(dest, name, remLen);
        dest = atoa(dest, "\":{", remLen);
    } else {
        dest = chtoa(dest, '{', remLen);
    }
    return dest;
}

char* json_objClose(char* dest, size_t* remLen) {
    if (dest[-1] == ',') {
        dest[-1] = '\0';
        (*remLen)++;
    }
    dest = atoa(dest, "},", remLen);
    return dest;
}