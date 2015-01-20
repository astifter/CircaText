#include <stdio.h>
#include "stringbuffer.h"

void stringbuffer_init(stringbuffer* sb) {
    sb->retval[0] = '\0';
    sb->current = sb->retval;
    sb->free = MAXLEN - 1;
}
void stringbuffer_append(stringbuffer* sb, char* value) {
    if (sb->free <= 0) return;
    unsigned int new = snprintf(sb->current, sb->free+1, "%s", value);
    sb->current += new;
    sb->free -= new;
}
