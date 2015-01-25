#include <pebble.h>
#include "stringbuffer.h"
#include "storage.h"
#include "appsync.h"
#include "hw_handling.h"
#include "main_window.h"

static void appsync_value_changed_callback(void) {
    const char* text = appsync_values.text;
    flash_text(text);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void tick_handler(struct tm *tick_time, TimeUnits units_changed)");
    // Hand updating to separate method, this method is used at watchface
    // loading as well.
    update_time();
}

static void handle_init(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void handle_init(void)");

    storage_init();
    bt_state_init(update_time);
    battery_state_init(update_time);
    appsync_init(appsync_value_changed_callback);

    main_window_create();

    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void handle_deinit(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void handle_deinit(void)");

    tick_timer_service_unsubscribe();

    main_window_destroy();

    appsync_deinit();
    battery_state_deinit();
    bt_state_deinit();
    storage_deinit();
}

int main(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "int main(void)");

    handle_init();
    app_event_loop();
    handle_deinit();
}
