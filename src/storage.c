#include <pebble.h>
#include "storage.h"
#include "stringbuffer.h"

// Indices to storage and data to access copies of stored values.
enum {
  SELECTED_VERSION = 0x0,
  BATTERY_ESTIMATE = 0x10001,
};
storage_t storage;

static void app_log_storage_log(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.selectedVersion: %s", storage.selectedVersion);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state_timestamp: %ld", storage.battery_estimate.previous_state_timestamp);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state.charge_percent: %d", storage.battery_estimate.previous_state.charge_percent);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state.is_charging: %d", storage.battery_estimate.previous_state.is_charging);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.previous_state.is_plugged: %d", storage.battery_estimate.previous_state.is_plugged);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.average_data_write_head: %d", storage.battery_estimate.average_data_write_head);
    for (int i = 0; i < battery_estimate_data_average_data_num; i++) {
        //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.battery_estimate.averate_data[%d]: %ld", i, storage.battery_estimate.averate_data[i]);
    }
}

// Makes sure storage is populated (by checking and, on absence, writing
// defaults) and then reading values.
void storage_init(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init()");

    if (!persist_exists(SELECTED_VERSION)) {
        persist_write_string(SELECTED_VERSION, "Regular");
    }
    persist_read_string(SELECTED_VERSION, storage.selectedVersion, 64);
    if (!persist_exists(BATTERY_ESTIMATE)) {
        persist_write_data(BATTERY_ESTIMATE, (void*)&battery_estimate_data_init, sizeof(battery_estimate_data));
    }
    persist_read_data(BATTERY_ESTIMATE, (void*)&(storage.battery_estimate), sizeof(battery_estimate_data));

    //app_log_storage_log();
}
    
// Write back values.
void storage_persist(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_persist()");

    persist_write_string(SELECTED_VERSION, storage.selectedVersion);
    persist_write_data(BATTERY_ESTIMATE, (void*)&(storage.battery_estimate), sizeof(battery_estimate_data));

    //app_log_storage_log();
}

// Deinitalization, currently just writes back values but might do more in the
// future.
void storage_deinit(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_deinit()");

    storage_persist();
}
