#include "../include/sort/quick_sort.h"
#include <stdio.h>

/**
 * @brief Swaps two elements in an array.
 * 
 * @param a Pointer to the first element.
 * @param b Pointer to the second element.
 */
void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Partitions the array for Quick Sort.
 * 
 * This function rearranges elements in the array so that elements less than or greater than
 * the pivot are positioned correctly according to the specified order.
 * 
 * @param arr Array to be partitioned.
 * @param low Starting index of the portion to partition.
 * @param high Ending index of the portion to partition.
 * @param ascending Sorting order: if non-zero, partitions for ascending order;
 *                  otherwise, partitions for descending order.
 * @return Index of the pivot element after partitioning.
 */
int partition(double arr[], int low, int high, int ascending) {
    double pivot = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        if (ascending) {
            while (arr[i] <= pivot && i <= high - 1) {
                i++;
            }
            while (arr[j] > pivot && j >= low + 1) {
                j--;
            }
        } else {
            while (arr[i] >= pivot && i <= high - 1) {
                i++;
            }
            while (arr[j] < pivot && j >= low + 1) {
                j--;
            }
        }

        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[low], &arr[j]);
    return j;
}

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
void quickSort(double arr[], int low, int high, int ascending) {
    if (low < high) {
        int pi = partition(arr, low, high, ascending);
        quickSort(arr, low, pi - 1, ascending);
        quickSort(arr, pi + 1, high, ascending);
    }
}
