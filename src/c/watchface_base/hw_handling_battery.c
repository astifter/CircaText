#include <pebble.h>
#include "stringbuffer.h"
#include "hw_handling.h"
#include "battery_estimate.h"
#include "logging_helper.h"
#include "storage.h"

// States and string descriptions of that states and the respective callbacks.
static BatteryChargeState        battery_state;
static stringbuffer              battery_state_sb;
static hardware_changed_callback battery_state_changed_callback;

// THIS MUST NOT BE ENABLED static BatteryChargeState test_battery_state = { 100, 0, 0 };

char* battery_state_string(void) {
    LOG_FUNC();

    stringbuffer_init(&battery_state_sb);
    if (storage.battery_display & battery_display_level) {
        stringbuffer_append_fi(&battery_state_sb, "%d%%", battery_state.charge_percent);

        if (battery_state.is_plugged) {
            if (battery_state.is_charging) {
                stringbuffer_append(&battery_state_sb, " | p,c");
            } else {
                stringbuffer_append(&battery_state_sb, " | p");
            }
        } else {
            stringbuffer_append_fs(&battery_state_sb, "%s", battery_estimate_string());
        }
    }

    return battery_state_sb.retval;
}

// The hardware handlers operate after the same principle:
// - The initializer reads the state directly, uses the handler to update the string description and registers the hw-callback handler.
// - The handler updates the string from the received values and calls the registered callback.
// - The deinitializer deregisters the callback.
static void handle_battery_event(BatteryChargeState s) {
    LOG_FUNC();

    battery_state = s;
    battery_estimate_update(s);

    battery_state_changed_callback();
}

// THIS MUST NOT BE ENABLED static void test_timer(void* data) {
// THIS MUST NOT BE ENABLED     LOG(LOG_ALL, "WARNING: THIS SHOULD NOT APPEAR!")
// THIS MUST NOT BE ENABLED     test_battery_state.charge_percent -= 10;
// THIS MUST NOT BE ENABLED     handle_battery_event(test_battery_state);
// THIS MUST NOT BE ENABLED
// THIS MUST NOT BE ENABLED     app_timer_register(15000, test_timer, NULL);
// THIS MUST NOT BE ENABLED }

void battery_state_init(hardware_changed_callback c) {
    LOG_FUNC();

    battery_estimate_init();

    battery_state_changed_callback = c;
    battery_state = battery_state_service_peek();
    handle_battery_event(battery_state);
    battery_state_service_subscribe(handle_battery_event);

    // THIS MUST NOT BE ENABLED app_timer_register(15000, test_timer, NULL);
}

void battery_state_deinit(void) {
    LOG_FUNC();

    battery_state_service_unsubscribe();
}
