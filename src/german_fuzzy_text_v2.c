#include <stdio.h>

#include "watchface_base/stringbuffer.h"
#include "watchface_base/logging_helper.h"

#include "german_fuzzy_text_common.h"

static stringbuffer sbval;

// Takes hours and minutes and converts them to a German time text.
// This is the version that provides a shorter version of the time, with
// "fünf vor" and "fünf nach" of "viertel" and "dreiviertel" replaced.
char* german_fuzzy_text_v2(int hour, int minute) {
    LOG_FUNC();

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

    // First divide minutes into 12 sections, then for the sections use
    // different terms.  "fünf vor", for the sections 2, 5, 8, 11 use "fünf
    int section = minute / 5;

    if (german_fuzzy_text_last_section == section) { german_fuzzy_text_dirty = 0; return sbval.retval; }
    german_fuzzy_text_last_section = section;

    // Prepare string for returning, reset used and free counters.
    stringbuffer_init(&sbval);

    if (section % 6 == 1)
        stringbuffer_append(&sbval, "zehn vor\n");
    if (section % 6 == 2)
        stringbuffer_append(&sbval, "fünf vor\n");
    if (section % 6 == 4)
        stringbuffer_append(&sbval, "fünf nach\n");
    if (section % 6 == 5)
        stringbuffer_append(&sbval, "zehn nach\n");

    // Use the sections again to fetch the next part of the string.
    if (section == 0)
        stringbuffer_append(&sbval, "viertel\n");
    if (section > 0 && section < 6)
        stringbuffer_append(&sbval, "halb\n");
    if (section == 6)
        stringbuffer_append(&sbval, "dreiviertel\n");
    // (section % 4 == 3) is the full hour.

    // This format is inherently 12 hour based, make sure hour is corrected and
    // printed accordingly.
    if (hour >= 12) hour -= 12;
    stringbuffer_append(&sbval, german_numbers[hour+1]);

    german_fuzzy_text_dirty = 1;
    return sbval.retval;
}
