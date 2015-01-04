#include <stdio.h>

#include "german_fuzzy_text_v1.h"
#include "german_fuzzy_text_v2.h"

int main (int argc, char** argv) {
    tttp func = 0;
    if (argc > 1 && (argv[1][0] == '-' && argv[1][1] == '2')) {
        func = german_fuzzy_text_v2;
    } else {
        func = german_fuzzy_text;
    }
    for (int hour = 0; hour < 24; hour++) {
        for (int min = 0; min < 60; min++) {
            char* string = func(hour, min);
            printf("--- %2d:%02d ---\n%s\n", hour, min, string);
        }
    }
    return 0;
}
