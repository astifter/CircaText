#include <stdio.h>

// Include the stringbuffer handling.
#include "stringbuffer.h"
static stringbuffer sbval;

// Takes hours and minutes and converts them to a German time text.
// This is the version that provides a more precise (albeit more verbose)
// version of the time.
char* german_fuzzy_text(int hour, int minute) {
    // Prepare string for returning, reset used and free counters.
    stringbuffer_init(&sbval);

    // To make things easier the first eight minutes of the new hour are
    // handled together with the previous hour. For this the minutes are
    // shifted back by 8 correcting the hour accordingly.
    minute -= 8;
    if (minute < 0) {
        minute += 60;
        hour -= 1;
    }
    if (hour < 0) {
        hour += 24;
    }

    // First divide minutes into 12 sections, for the sections 0, 3, 6, 9 use
    // "fünf vor", for the sections 2, 5, 8, 11 use "fünf nach".
    int section = minute / 5;
    if (section % 3 == 0)
        stringbuffer_append(&sbval, "fünf vor\n");
    if (section % 3 == 2)
        stringbuffer_append(&sbval, "fünf nach\n");

    // Now merge the sections into 4 blocks, use "viertel", "halb" and
    // "dreiviertel" accordingly.
    section = section / 3;
    if (section % 4 == 0)
        stringbuffer_append(&sbval, "viertel\n");
    if (section % 4 == 1)
        stringbuffer_append(&sbval, "halb\n");
    if (section % 4 == 2)
        stringbuffer_append(&sbval, "dreiviertel\n");

    // This format is inherently 12 hour based, make sure hour is corrected and
    // printed accordingly.
    if (hour >= 12) hour -= 12;
    switch (hour+1) {
        case 1:  stringbuffer_append(&sbval, "eins"); break;
        case 2:  stringbuffer_append(&sbval, "zwei"); break;
        case 3:  stringbuffer_append(&sbval, "drei"); break;
        case 4:  stringbuffer_append(&sbval, "vier"); break;
        case 5:  stringbuffer_append(&sbval, "fünf"); break;
        case 6:  stringbuffer_append(&sbval, "sechs"); break;
        case 7:  stringbuffer_append(&sbval, "sieben"); break;
        case 8:  stringbuffer_append(&sbval, "acht"); break;
        case 9:  stringbuffer_append(&sbval, "neun"); break;
        case 10: stringbuffer_append(&sbval, "zehn"); break;
        case 11: stringbuffer_append(&sbval, "elf"); break;
        case 12: stringbuffer_append(&sbval, "zwölf"); break;
    }

    return sbval.retval;
}
