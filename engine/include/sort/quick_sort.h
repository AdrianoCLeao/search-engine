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

#ifndef QUICK_SORT_H
#define QUICK_SORT_H

/**
 * @brief A structure to represent a document and its score.
 */
typedef struct {
    char name[32];
    double score;
} DocumentScore;

/**
 * @brief Sorts an array of doubles using the Quick Sort algorithm.
 * 
 * This function implements the Quick Sort algorithm to sort an array of doubles.
 * The sorting order can be specified as ascending or descending.
 * 
 * @param arr Array of doubles to be sorted.
 * @param low Starting index of the portion to sort.
 * @param high Ending index of the portion to sort.
 * @param ascending Sorting order: if non-zero, sorts in ascending order;
 *                  otherwise, sorts in descending order.
 */
void quickSort(double arr[], int low, int high, int ascending);

#endif 
