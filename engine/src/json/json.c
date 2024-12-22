#include "../include/json/json.h"
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

char* json_arrOpen(char* dest, const char* name, size_t* remLen) {
    if (name) {
        dest = chtoa(dest, '\"', remLen);
        dest = atoa(dest, name, remLen);
        dest = atoa(dest, "\":[", remLen);
    } else {
        dest = chtoa(dest, '[', remLen);
    }
    return dest;
}

char* json_arrClose(char* dest, size_t* remLen) {
    if (dest[-1] == ',') {
        dest[-1] = '\0';
        (*remLen)++;
    }
    dest = atoa(dest, "],", remLen);
    return dest;
}

char* json_nstr(char* dest, const char* name, const char* value, size_t* remLen) {
    if (name) {
        dest = chtoa(dest, '\"', remLen);
        dest = atoa(dest, name, remLen);
        dest = atoa(dest, "\":\"", remLen);
    }
    dest = atoa(dest, value, remLen);
    dest = atoa(dest, "\",", remLen);
    return dest;
}

char* json_num(char* dest, const char* name, double value, size_t* remLen) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%g", value);
    if (name) {
        dest = chtoa(dest, '\"', remLen);
        dest = atoa(dest, name, remLen);
        dest = atoa(dest, "\":", remLen);
    }
    dest = atoa(dest, buffer, remLen);
    dest = chtoa(dest, ',', remLen);
    return dest;
}

char* json_bool(char* dest, const char* name, int value, size_t* remLen) {
    if (name) {
        dest = chtoa(dest, '\"', remLen);
        dest = atoa(dest, name, remLen);
        dest = atoa(dest, "\":", remLen);
    }
    dest = atoa(dest, value ? "true" : "false", remLen);
    dest = chtoa(dest, ',', remLen);
    return dest;
}

char* json_finalize(char* dest, size_t* remLen) {
    if (dest[-1] == ',') {
        dest[-1] = '\0';
        (*remLen)++;
    }
    return dest;
}