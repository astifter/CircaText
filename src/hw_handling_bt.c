#include <pebble.h>
#include "stringbuffer.h"
#include "hw_handling.h"

// States and string descriptions of that states and the respective callbacks.
static bool               bt_state;
static stringbuffer       bt_state_sb;
static hardware_changed_callback bt_state_changed_callback;
char* bt_state_string;
int   bt_state_string_dirty;

// The hardware handlers operate after the same principle:
// - The initializer reads the state directly, uses the handler to update the string description and registers the hw-callback handler.
// - The handler updates the string from the received values and calls the registered callback.
// - The deinitializer deregisters the callback.
static void handle_bt_event(bool connected) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void handle_bt_event(bool connected)");
    bt_state = connected;

    stringbuffer_init(&bt_state_sb);
    stringbuffer_append(&bt_state_sb, "bt: ");
    if (bt_state) {
        stringbuffer_append(&bt_state_sb, "conn");
    } else {
        stringbuffer_append(&bt_state_sb, "disconn");
    }

    bt_state_string = bt_state_sb.retval;
    bt_state_string_dirty = 1;
    bt_state_changed_callback();
}

void bt_state_init(hardware_changed_callback c) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void bt_state_init(hardware_changed_callback c)");
    bt_state_changed_callback = c;
    bt_state = bluetooth_connection_service_peek();
    handle_bt_event(bt_state);
    bluetooth_connection_service_subscribe(handle_bt_event);
}

void bt_state_deinit(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void bt_state_deinit(void)");
    bluetooth_connection_service_unsubscribe();
}
