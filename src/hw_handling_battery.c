#include <pebble.h>
#include "stringbuffer.h"
#include "hw_handling.h"
#include "battery_estimate.h"

// States and string descriptions of that states and the respective callbacks.
static BatteryChargeState battery_state;
static stringbuffer       battery_state_string;
static hardware_changed_callback battery_state_changed_callback;

// THIS MUST NOT BE ENABLED static BatteryChargeState test_battery_state = { 100, 0, 0 };

// The hardware handlers operate after the same principle:
// - The initializer reads the state directly, uses the handler to update the string description and registers the hw-callback handler.
// - The handler updates the string from the received values and calls the registered callback.
// - The deinitializer deregisters the callback.
static void handle_battery_event(BatteryChargeState s) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void handle_battery_event(BatteryChargeState s)");

    battery_state = s;
    battery_estimate_update(s);
    
    stringbuffer_init(&battery_state_string);
    stringbuffer_append_fi(&battery_state_string, "%d%%", battery_state.charge_percent);    

    if (battery_state.is_plugged) {
        if (battery_state.is_charging) {
            stringbuffer_append(&battery_state_string, " (p, c)");
        } else {
            stringbuffer_append(&battery_state_string, " (p)");
        }
    } else {
        stringbuffer_append_fs(&battery_state_string, " (%s)", battery_estimate_string());
    }

    battery_state_changed_callback();
}

// THIS MUST NOT BE ENABLED static void test_timer(void* data) {
// THIS MUST NOT BE ENABLED     test_battery_state.charge_percent -= 10;
// THIS MUST NOT BE ENABLED     handle_battery_event(test_battery_state);
// THIS MUST NOT BE ENABLED 
// THIS MUST NOT BE ENABLED     app_timer_register(15000, test_timer, NULL);
// THIS MUST NOT BE ENABLED }

void battery_state_init(hardware_changed_callback c) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void battery_state_init(hardware_changed_callback c)");

    battery_estimate_init();

    battery_state_changed_callback = c;
    battery_state = battery_state_service_peek();
    handle_battery_event(battery_state);
    battery_state_service_subscribe(handle_battery_event);

    battery_estimate_unlock();

    // THIS MUST NOT BE ENABLED app_timer_register(15000, test_timer, NULL);
}

void battery_state_deinit(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void battery_state_deinit(void)");

    battery_state_service_unsubscribe();
}

// The functions to return the state description respectively.
char* get_battery_state(void) {
    return battery_state_string.retval;
}

