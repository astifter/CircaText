#include <stdio.h>
#include <string.h>

#include "stringbuffer.h"
stringbuffer sbval;

int main(int argc, char** argv) {
    stringbuffer_init(&sbval);
    printf("stringbuffer:\n%s\n", sbval.retval);
    printf("strlen(stringbuffer):%lu\n", strlen(sbval.retval));
    for (int i = 0; i < 13; i++) {
        stringbuffer_append(&sbval, "0123456789");
    }
    printf("stringbuffer:\n%s\n", sbval.retval);
    printf("strlen(stringbuffer):%lu\n", strlen(sbval.retval));
    printf("free:%d\n", sbval.free);
}
