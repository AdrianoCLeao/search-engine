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

#ifndef JSON_H
#define JSON_H

#include <stddef.h>

/**
 * @brief Opens a JSON object.
 *
 * This function adds an opening brace (`{`) for a JSON object and optionally
 * a name for the object in the format `"name":{`.
 *
 * @param dest Pointer to the destination string buffer.
 * @param name Name of the JSON object (optional).
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_objOpen(char* dest, const char* name, size_t* remLen);

/**
 * @brief Closes a JSON object.
 *
 * This function adds a closing brace (`}`) for a JSON object and removes
 * trailing commas if present.
 *
 * @param dest Pointer to the destination string buffer.
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_objClose(char* dest, size_t* remLen);

/**
 * @brief Opens a JSON array.
 *
 * This function adds an opening bracket (`[`) for a JSON array and optionally
 * a name for the array in the format `"name":[`.
 *
 * @param dest Pointer to the destination string buffer.
 * @param name Name of the JSON array (optional).
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_arrOpen(char* dest, const char* name, size_t* remLen);

/**
 * @brief Closes a JSON array.
 *
 * This function adds a closing bracket (`]`) for a JSON array and removes
 * trailing commas if present.
 *
 * @param dest Pointer to the destination string buffer.
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_arrClose(char* dest, size_t* remLen);

/**
 * @brief Adds a JSON name-value pair with a string value.
 *
 * This function adds a name-value pair in the format `"name":"value",`.
 *
 * @param dest Pointer to the destination string buffer.
 * @param name Name of the JSON field.
 * @param value String value for the field.
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_nstr(char* dest, const char* name, const char* value, size_t* remLen);

/**
 * @brief Adds a JSON name-value pair with a numeric value.
 *
 * This function adds a name-value pair in the format `"name":value,`.
 *
 * @param dest Pointer to the destination string buffer.
 * @param name Name of the JSON field.
 * @param value Numeric value for the field.
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_num(char* dest, const char* name, double value, size_t* remLen);

/**
 * @brief Adds a JSON name-value pair with a boolean value.
 *
 * This function adds a name-value pair in the format `"name":true,` or
 * `"name":false,`.
 *
 * @param dest Pointer to the destination string buffer.
 * @param name Name of the JSON field.
 * @param value Boolean value for the field (0 for false, non-zero for true).
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_bool(char* dest, const char* name, int value, size_t* remLen);

/**
 * @brief Finalizes a JSON structure.
 *
 * This function removes any trailing commas from the JSON structure.
 *
 * @param dest Pointer to the destination string buffer.
 * @param remLen Pointer to the remaining buffer length.
 * @return Pointer to the destination buffer after the operation.
 */
char* json_finalize(char* dest, size_t* remLen);

#endif
