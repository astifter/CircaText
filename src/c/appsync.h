#pragma once

#include "timetotextpointer.h"

// The synced data and a text representation of the settings.
typedef struct {
    tttp time_to_text_pointer;
    char* text;
} appsync_values_t;
extern appsync_values_t appsync_values;

// The function pointer for the callback that is called when the app_sync
// values might have been changed.
typedef void (*appsync_callback)(void);

// The appsync initialization and deinit.
void appsync_init(appsync_callback c);
void appsync_deinit(void);
