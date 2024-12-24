/*
 * This file is part of TFIDF Search Engine.
 *
 * Copyright (C) 2024 Adriano Leão
 *
 * TFIDF Search Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TFIDF Search Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TFIDF Search Engine. If not, see <https://www.gnu.org/licenses/>.
 */
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

char* json_objClose( char* dest, size_t* remLen  ) {
    if ( dest[-1] == ',' )
    {
        --dest;
        ++*remLen;
    }
    return atoa( dest, "},", remLen );
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

char* json_finalize( char* dest, size_t* remLen ) {
    if ( ',' == dest[-1] ) {
        dest[-1] = '\0';
        --dest;
        ++*remLen;
    }

    return dest;
}