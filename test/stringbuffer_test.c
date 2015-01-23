#include <stdio.h>
#include <string.h>

#include "stringbuffer.h"
stringbuffer sbval;

void printvalues(stringbuffer* sb) {
    printf("stringbuffer:\n%s\n", sb->retval);
    printf("strlen(stringbuffer):%lu\n", strlen(sb->retval));
    printf("free:%d\n", sb->free);
}

int main(int argc, char** argv) {
    // initialize stringbuffer
    stringbuffer_init(&sbval);
    // print initialized state
    printvalues(&sbval);

    // now append more that the string can hold
    for (int i = 0; i < 13; i++) {
        stringbuffer_append(&sbval, "01234567");
        stringbuffer_append_int(&sbval, 89);
    }
    // print values from full string
    printvalues(&sbval);
}
