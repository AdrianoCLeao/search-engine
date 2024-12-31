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

#include "../include/core/engine.h"
#include "../include/utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    clear_screen();
    TFIDFEngine engine;

    /* tfidf_initialize(&engine);
    tfidf_load_documents(&engine, "../data/wiki");

    tfidf_calculate(&engine); */

    char query[256];
    printf("Digite a consulta: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    tfidf_search(&engine, query);

    /* tfidf_free(&engine); */
    
    return 0;
}
