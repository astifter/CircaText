#include <pebble.h>
#include "german_fuzzy_text_v1.h"
#include "german_fuzzy_text_v2.h"
#include "stringbuffer.h"
#include "storage.h"
#include "appsync.h"

static AppSync sync;
static uint8_t sync_buffer[124];
static stringbuffer sb;

appsync_values_t appsync_values;
appsync_callback callback;

enum {
    SELECTED_VERISON = 0x0,
};

static void populate_values(void) {
    stringbuffer_init(&sb);
    stringbuffer_append(&sb, "Einstellungen:\n");
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "populate_values(void)");
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
}

static void sync_tuple_changed_callback(const uint32_t key, 
                                        const Tuple* new_tuple, 
                                        const Tuple* old_tuple, 
                                        void* context) {
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "key: %u", (unsigned int)key);
    switch (key) {
        case SELECTED_VERISON: {
            strcpy(storage.selectedVersion, new_tuple->value->cstring);
            storage_persist();
            populate_values();
        } break;
    }
    callback();
}

// http://stackoverflow.com/questions/21150193/logging-enums-on-the-pebble-watch/21172222#21172222
static char *translate_error(AppMessageResult result) {
    switch (result) {
        case APP_MSG_OK: return "APP_MSG_OK";
        case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
        case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
        case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
        case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
        case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
        case APP_MSG_BUSY: return "APP_MSG_BUSY";
        case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
        case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
        case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
        case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
        case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
        case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
        case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
        default: return "UNKNOWN ERROR";
    }
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "sync error: %s", translate_error(app_message_error));
}

void appsync_init(appsync_callback c) {
    appsync_values.time_to_text_pointer = german_fuzzy_text;
    callback = c;

    Tuplet initial_values[] = {
       TupletCString(SELECTED_VERISON, (const char*)(storage.selectedVersion))
    };
  
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "appsync_init()");
    app_sync_init(&sync, 
                  sync_buffer, sizeof(sync_buffer), 
                  initial_values, ARRAY_LENGTH(initial_values),
                  sync_tuple_changed_callback, sync_error_callback, NULL);

    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "app_message_open()");
    app_message_open(124, 124);
}

void appsync_deinit(void) {
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "appsync_deinit()");
    app_sync_deinit(&sync);
}
