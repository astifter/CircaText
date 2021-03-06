#include <pebble.h>

#include "watchface_base/watchface_base.h"
#include "watchface_base/logging_helper.h"

#include "appsync.h"
#include "main_window.h"

static void appsync_value_changed_callback(void) {
    LOG_FUNC();

    flash_text(appsync_values.text);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    LOG_FUNC();

    update_time();
}

static void handle_init(void) {
    LOG_FUNC();

    watchface_base_init(update_time);
    appsync_init(appsync_value_changed_callback);

    main_window_create();

    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void handle_deinit(void) {
    LOG_FUNC();

    tick_timer_service_unsubscribe();

    main_window_destroy();

    appsync_deinit();
    watchface_base_deinit();
}

int main(void) {
    LOG_FUNC();

    handle_init();
    app_event_loop();
    handle_deinit();
}
