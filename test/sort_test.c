#include "watchface_base/quicksort.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 40

int compar(const void* f, const void* s) {
    const time_t first = *(const time_t*)f;
    const time_t secnd = *(const time_t*)s;
    if (first < secnd) return -1;
    if (first > secnd) return  1;
    return 0;
}

void print(const char* m, time_t* data, int s) {
    printf("%s:", m);
    for (int i = 0; i < s; i++) {
        printf("%ld,", data[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    srand (time(NULL));
    time_t data[SIZE];
    time_t rata[SIZE];

    for (int i = 0; i<SIZE; i++) {
        data[i] = rand() % SIZE;
        rata[i] = data[i];
    }
    print("data", data, SIZE);
    print("rata", rata, SIZE);

    quicksort(data, 0, SIZE-1);
    qsort(rata, SIZE, sizeof(time_t), compar);

    print("data", data, SIZE);
    print("rata", rata, SIZE);

    for (int i = 0; i<SIZE; i++) {
        if (data[i] != rata[i])
            return -1;
        if (((i+1) < SIZE) && (data[i] > data[i+1]))
            return -1;
    }
}
