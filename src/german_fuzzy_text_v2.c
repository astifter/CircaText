#include <stdio.h>

// Create static buffer to return to caller.
#define MAXLEN 80
static char retval[MAXLEN];

// Helper function for appending strings together.
static void addstring(char* dest, char* src, int* used, int* free) {
    (*used) += snprintf(dest + (*used), (*free), "%s", src);
    (*free) = MAXLEN - 1 - (*used);
}

// Takes hours and minutes and converts them to a German time text.
char* german_fuzzy_text_v2(int hour, int minute) {
    // Prepare string for returning, reset used and free counters.
    int used = 0;
    int free = MAXLEN - 1 - used;
    retval[used] = '\0';

    // To make things easier the first 13 minutes of the new hour are
    // handled together with the previous hour. For this the minutes are
    // shifted back by 13 correcting the hour accordingly.
    minute -= 13;
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
    if (section % 6 == 1)
        addstring(retval, "zehn vor\n", &used, &free);
    if (section % 6 == 2)
        addstring(retval, "fünf vor\n", &used, &free);
    if (section % 6 == 4)
        addstring(retval, "fünf nach\n", &used, &free);
    if (section % 6 == 5)
        addstring(retval, "zehn nach\n", &used, &free);

    // Now merge the sections into 4 blocks, use "viertel", "halb" and
    // "dreiviertel" accordingly.
    if (section == 0)
        addstring(retval, "viertel\n", &used, &free);
    if (section > 0 && section < 6)
        addstring(retval, "halb\n", &used, &free);
    if (section == 6)
        addstring(retval, "dreiviertel\n", &used, &free);
    // (section % 4 == 3) is the full hour.

    // This format is inherently 12 hour based, make sure hour is corrected and
    // printed accordingly.
    if (hour >= 12) hour -= 12;
    switch (hour+1) {
        case 1:  addstring(retval, "eins", &used, &free); break;
        case 2:  addstring(retval, "zwei", &used, &free); break;
        case 3:  addstring(retval, "drei", &used, &free); break;
        case 4:  addstring(retval, "vier", &used, &free); break;
        case 5:  addstring(retval, "fünf", &used, &free); break;
        case 6:  addstring(retval, "sechs", &used, &free); break;
        case 7:  addstring(retval, "sieben", &used, &free); break;
        case 8:  addstring(retval, "acht", &used, &free); break;
        case 9:  addstring(retval, "neun", &used, &free); break;
        case 10: addstring(retval, "zehn", &used, &free); break;
        case 11: addstring(retval, "elf", &used, &free); break;
        case 12: addstring(retval, "zwölf", &used, &free); break;
    }

    return retval;
}
