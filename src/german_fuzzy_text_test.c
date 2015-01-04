#include <stdio.h>

#include "german_fuzzy_text.h"

int main (int argc, char** argv) {
    for (int hour = 0; hour < 24; hour++) {
        for (int min = 0; min < 60; min++) {
            char* string = german_fuzzy_text(hour, min);
            printf("%2d:%02d %s\n", hour, min, string);
        }
    }
    return 0;
}
