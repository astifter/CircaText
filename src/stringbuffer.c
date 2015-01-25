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
    if (sb->free <= 0) return;
    unsigned int new = snprintf(sb->current, sb->free+1, "%s", value);
    stringbuffer_update_counters(sb, new);
}
void stringbuffer_append_int(stringbuffer* sb, int value) {
    if (sb->free <= 0) return;
    unsigned int new = snprintf(sb->current, sb->free+1, "%d", value);
    stringbuffer_update_counters(sb, new);
}
