#include <stdio.h>
#include "stringbuffer.h"

// Take a stringbuffer and initialize it:
// - delete string and write first '\0' byte
// - set the current pointer to the start of the string
// - reset free counter.
void stringbuffer_init(stringbuffer* sb) {
    sb->retval[0] = '\0';
    sb->current = sb->retval;
    sb->free = MAXLEN - 1;
}

static void stringbuffer_update_counters(stringbuffer* sb, unsigned int new) {
    sb->current += new;
    sb->free -= new;
}

// If buffer is still not full, write value to it and update current pointer
// and free counter.
void stringbuffer_append(stringbuffer* sb, const char* value) {
    stringbuffer_append_str(sb, value);
}
void stringbuffer_append_str(stringbuffer* sb, const char* value) {
    stringbuffer_append_fs(sb, "%s", value);
}
void stringbuffer_append_fs(stringbuffer* sb, const char* fmt, const char* value) {
    if (sb->free <= 0) return;
    unsigned int new = snprintf(sb->current, sb->free+1, fmt, value);
    stringbuffer_update_counters(sb, new);
}

void stringbuffer_append_int(stringbuffer* sb, int value) {
    stringbuffer_append_fi(sb, "%d", value);
}
void stringbuffer_append_fi(stringbuffer* sb, const char* fmt, const int value) {
    if (sb->free <= 0) return;
    unsigned int new = snprintf(sb->current, sb->free+1, fmt, value);
    stringbuffer_update_counters(sb, new);
}

void stringbuffer_append_ti(stringbuffer* sb, unsigned int value) {
    unsigned int seconds = value % 60; value /= 60;
    unsigned int minutes = value % 60; value /= 60;
    unsigned int hours   = value % 24; value /= 24;
    unsigned int days    = value;

    if (days == 0 && hours == 0) {
        stringbuffer_append_fi(sb, "%dm", minutes);
        stringbuffer_append_fi(sb, "%d", seconds);
    } else if (days == 0) {
        stringbuffer_append_fi(sb, "%dh", hours);
        stringbuffer_append_fi(sb, "%d", minutes);
    } else {
        stringbuffer_append_fi(sb, "%dd", days);
        stringbuffer_append_fi(sb, "%d", hours);
    }
}
