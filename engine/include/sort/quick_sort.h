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
