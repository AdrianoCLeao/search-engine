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

#include "tf-idf/tfidf_engine.h"

void tfidf_initialize(TFIDFEngine *engine);
void tfidf_load_documents(TFIDFEngine *engine, const char *directory);
void tfidf_calculate(TFIDFEngine *engine);
void tfidf_search(TFIDFEngine *engine, const char *query);
void tfidf_free(TFIDFEngine *engine);

#endif
