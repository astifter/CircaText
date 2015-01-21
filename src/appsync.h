#pragma once

#include "timetotextpointer.h"

typedef struct {
    tttp time_to_text_pointer;
    char* text;
} appsync_values_t;
extern appsync_values_t appsync_values;

typedef void (*appsync_callback)(void);

void appsync_init(appsync_callback c);
void appsync_deinit(void);
