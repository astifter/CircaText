#pragma once

// Buffers can hold at most MAXLEN-1 characters.
#define MAXLEN 80

// This is a stringbuffer, it keeps the buffer, the pointer to the last byte
// and the free byte count.
typedef struct {
    char retval[MAXLEN];
    char* current;
    int free;
} stringbuffer;

// Initializes the stringbuffer.
void stringbuffer_init(stringbuffer* sb);
// Appends string value to stringbuffer.
void stringbuffer_append(stringbuffer* sb, char* value);
