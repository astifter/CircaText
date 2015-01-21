#include <stdio.h>

#include "german_fuzzy_text_v1.h"
#include "german_fuzzy_text_v2.h"
#include "time_to_string_function.h"

int main (int argc, char** argv) {
    // Depending on argument select time to text function.
    tttp func = 0;
    if (argc > 1 && (argv[1][0] == '-' && argv[1][1] == '2')) {
        func = german_fuzzy_text_v2;
    } else {
        func = german_fuzzy_text;
    }

    // Then, for each hour and minute use the function to print time string.
    for (int hour = 0; hour < 24; hour++) {
        for (int min = 0; min < 60; min++) {
            char* string = func(hour, min);
            printf("--- %2d:%02d ---\n%s\n", hour, min, string);
        }
    }
    return 0;
}
