#include "../include/sort/quick_sort.h"
#include <stdio.h>

void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

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

void quickSort(double arr[], int low, int high, int ascending) {
    if (low < high) {
        int pi = partition(arr, low, high, ascending);
        quickSort(arr, low, pi - 1, ascending);
        quickSort(arr, pi + 1, high, ascending);
    }
}
