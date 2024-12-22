#ifndef QUICK_SORT_H
#define QUICK_SORT_H

typedef struct {
    char name[32];
    double score;
} DocumentScore;

void quickSort(double arr[], int low, int high);

#endif 
