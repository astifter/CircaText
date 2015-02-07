#include <pebble.h>
#include "german_fuzzy_text_v1.h"
#include "german_fuzzy_text_v2.h"
#include "stringbuffer.h"
#include "storage.h"
#include "appsync.h"
#include "logging_helper.h"

// Static variables for holding the synced state and the textual representation
// of the settings.
static AppSync sync;
static uint8_t sync_buffer[124];
static stringbuffer sb;

// The synced values and the changed callback.
appsync_values_t appsync_values;
appsync_callback callback;

enum {
    SELECTED_VERSION = 0x0,
};

// Reads back values from storage and sets "synced" values accordingly. Also
// populates textual representation of the values.
static void populate_values(void) {
    LOG_FUNC();

    stringbuffer_init(&sb);
    stringbuffer_append(&sb, "Einstellungen\n");

    stringbuffer_append(&sb, "Text: ");
    if (strcmp(storage.selectedVersion, "Regular") == 0) {
        appsync_values.time_to_text_pointer = german_fuzzy_text;
        stringbuffer_append(&sb, "Normal\n");
    }
    if (strcmp(storage.selectedVersion, "Short") == 0) {
        appsync_values.time_to_text_pointer = german_fuzzy_text_v2;
        stringbuffer_append(&sb, "Kurz\n");
    }

    appsync_values.text = sb.retval;
    LOG_EXT(LOG_APPSYNC, "settings string:\n%s", appsync_values.text);
}

// Called when the settings change, takes them and writes them into storage.
// Then calls populate_values() and calls callback function.
static void sync_tuple_changed_callback(const uint32_t key,
                                        const Tuple* new_tuple,
                                        const Tuple* old_tuple,
                                        void* context) {
    LOG_FUNC();

    switch (key) {
        case SELECTED_VERSION: {
            LOG_EXT(LOG_APPSYNC, "SELECTED_VERSION: %s", new_tuple->value->cstring);
            strcpy(storage.selectedVersion, new_tuple->value->cstring);
            storage_persist();
            populate_values();
        } break;
    }

    LOG(LOG_APPSYNC, "sync_tuple_changed_callback() exiting to callback");
    callback();
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    LOG_FUNC();
    LOG_EXT(LOG_APPSYNC, "sync error: %d", app_message_error);
}

// Initializes app_sync by populating values, storing callback, registering
// app_sync callbacks and opening messages.
void appsync_init(appsync_callback c) {
    LOG_FUNC();

    populate_values();
    callback = c;

    Tuplet initial_values[] = {
       TupletCString(SELECTED_VERSION, (const char*)(storage.selectedVersion))
    };

    LOG(LOG_APPSYNC, "app_sync_init()");
    app_sync_init(&sync,
                  sync_buffer, sizeof(sync_buffer),
                  initial_values, ARRAY_LENGTH(initial_values),
                  sync_tuple_changed_callback, sync_error_callback, NULL);

    LOG(LOG_APPSYNC, "app_message_open()");
    app_message_open(124, 124);
}

// Closes down app_sync.
void appsync_deinit(void) {
    LOG_FUNC();

    LOG(LOG_APPSYNC, "app_sync_deinit()");
    app_sync_deinit(&sync);
}
