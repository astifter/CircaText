#if LOG_TESTING
#include <stdio.h>
#endif
#include "quicksort.h"

int choose_pivot(int first, int last) {
    return (first + last) / 2;
}

void swap(time_t* data, int first, int second) {
    if (first == second) return;
    int temp = data[first];
    data[first] = data[second];
    data[second] = temp;
}

int partition(time_t* data, int first, int last) {
    int pivot_index = choose_pivot(first, last);
    int pivot_value = data[pivot_index];
    swap(data, pivot_index, last);
    int store_index = first;
    for (int i = first; i < last; i++) {
        if (data[i] < pivot_value) {
            swap(data, i, store_index);
            store_index++;
        }
    }
    swap(data, store_index, last);
    return store_index;
}

void quicksort(time_t* data, int first, int last) {
    if (first < last) {
//#if LOG_TESTING
//        printf("got: [%d,%d]", first, last);
//        for (int i = first; i <= last; i++) {
//            printf("%d,", data[i]);
//        }
//        printf("\n");
//#endif
        int p = partition(data, first, last);
        quicksort(data, first, p-1);
        quicksort(data, p+1, last);
//#if LOG_TESTING
//        printf("ret: [%d,%d]", first, last);
//        for (int i = first; i <= last; i++) {
//            printf("%d,", data[i]);
//        }
//        printf("\n");
//#endif
    }
}
