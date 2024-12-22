#ifndef JSON_H
#define JSON_H

#include <stddef.h>

char* json_objOpen(char* dest, const char* name, size_t* remLen);
char* json_objClose(char* dest, size_t* remLen);
char* json_arrOpen(char* dest, const char* name, size_t* remLen);
char* json_arrClose(char* dest, size_t* remLen);
char* json_nstr(char* dest, const char* name, const char* value, size_t* remLen);
char* json_num(char* dest, const char* name, double value, size_t* remLen);
char* json_bool(char* dest, const char* name, int value, size_t* remLen);
char* json_finalize(char* dest, size_t* remLen);

#endif
