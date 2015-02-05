#include <pebble.h>
#include "storage.h"
#include "stringbuffer.h"
#include "logging_helper.h"

// Indices to storage and data to access copies of stored values.
enum {
    SELECTED_VERSION = 0x0,
    BATTERY_ESTIMATE = 0x10001,
    LAST_FULL_TIMESTAMP = 0x10002,
    MAXIMUM_EVER_USED = 0x10002,
};
storage_t storage;

//static void app_log_storage_log(void* data) {
//    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.selectedVersion: %s", storage.selectedVersion);
//    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state_timestamp: %ld", storage.battery_estimate.previous_state_timestamp);
//    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state.charge_percent: %d", storage.battery_estimate.previous_state.charge_percent);
//    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state.is_charging: %d", storage.battery_estimate.previous_state.is_charging);
//    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state.is_plugged: %d", storage.battery_estimate.previous_state.is_plugged);
//    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.average_data_write_head: %d", storage.battery_estimate.average_data_write_head);
//    for (int i = 0; i < battery_estimate_data_average_data_num; i++) {
//        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.averate_data[%d]: %ld", i, storage.battery_estimate.averate_data[i]);
//    }
//}

// Makes sure storage is populated (by checking and, on absence, writing
// defaults) and then reading values.
void storage_init(void) {
    LOG_FUNC();

    // THIS MUST NEVER BE ENABLED */for (unsigned int i = 0; i <= MAXIMUM_EVER_USED; i++) {
    // THIS MUST NEVER BE ENABLED */    if ((i % 10000) == 0) {
    // THIS MUST NEVER BE ENABLED */        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init(): working key %d", i);
    // THIS MUST NEVER BE ENABLED */    }
    // THIS MUST NEVER BE ENABLED */    if (persist_exists(i)) {
    // THIS MUST NEVER BE ENABLED */        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init(): deleting key %d", i);
    // THIS MUST NEVER BE ENABLED */        persist_delete(i);
    // THIS MUST NEVER BE ENABLED */    }
    // THIS MUST NEVER BE ENABLED */}

    if (!persist_exists(SELECTED_VERSION)) {
        persist_write_string(SELECTED_VERSION, "Regular");
        //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init(): init %d with data %s", SELECTED_VERSION, "Regular");
    }
    persist_read_string(SELECTED_VERSION, storage.selectedVersion, 64);
    if (!persist_exists(BATTERY_ESTIMATE)) {
        persist_write_data(BATTERY_ESTIMATE, (void*)&battery_estimate_data_init, sizeof(battery_estimate_data));
        //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init(): init %d", BATTERY_ESTIMATE);
    }
    persist_read_data(BATTERY_ESTIMATE, (void*)&(storage.battery_estimate), sizeof(battery_estimate_data));
    if (!persist_exists(LAST_FULL_TIMESTAMP)) {
        time_t initial = (time_t)-1;
        persist_write_int(LAST_FULL_TIMESTAMP, initial);
        //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init(): init %d with data %d", BATTERY_ESTIMATE initial);
    }
    storage.last_full_timestamp = persist_read_int(LAST_FULL_TIMESTAMP);

    //app_timer_register(10000, app_log_storage_log, NULL);
}
    
// Write back values.
void storage_persist(void) {
    LOG_FUNC();

    persist_write_string(SELECTED_VERSION, storage.selectedVersion);
    persist_write_data(BATTERY_ESTIMATE, (void*)&(storage.battery_estimate), sizeof(battery_estimate_data));

    //app_log_storage_log();
}

// Deinitalization, currently just writes back values but might do more in the
// future.
void storage_deinit(void) {
    LOG_FUNC();

    storage_persist();
}
