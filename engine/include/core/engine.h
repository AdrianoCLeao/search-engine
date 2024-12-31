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

#ifndef ENGINE_H
#define ENGINE_H

/**
 * @brief Structure to represent a TF-IDF engine.
 * 
 * This structure stores the list of documents and the total number of documents
 * loaded into the engine for TF-IDF calculations.
 */
typedef struct {
    char **documents;  
    int num_documents; 
} TFIDFEngine;

/**
 * @brief Structure to represent data for a specific term.
 * 
 * This structure stores the term itself, its corresponding TF-IDF values for
 * all documents, and its IDF (Inverse Document Frequency).
 */
typedef struct {
    char *term;
    double *tfidf_values;
    double idf;
} TermData;

#define MAX_TERMS 10000

void tfidf_initialize(TFIDFEngine *engine);
void tfidf_load_documents(TFIDFEngine *engine, const char *directory);
void tfidf_calculate(TFIDFEngine *engine);
void tfidf_search(TFIDFEngine *engine, const char *query);
void tfidf_free(TFIDFEngine *engine);

#endif
