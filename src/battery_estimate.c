#include "battery_estimate.h"
#include "stringbuffer.h"
#include "battery_estimate_data.h"
#include "storage.h"
#include "logging_helper.h"

static stringbuffer battery_estimate_sb;

void battery_estimate_init(void) {
    LOG_FUNC();

    stringbuffer_init(&battery_estimate_sb);
}

static void battery_estimate_update_string(BatteryChargeState current) {
    LOG_FUNC();

    unsigned int sum = 0;
    for (int i = 0; i < battery_estimate_data_average_data_num; i++) {
        sum += storage.battery_estimate.averate_data[i];
    }
    unsigned int average = sum / battery_estimate_data_average_data_num;
    unsigned int remaining = current.charge_percent / 10;
    unsigned int remaining_secs = remaining * average;

    stringbuffer_init(&battery_estimate_sb);
    stringbuffer_append_str(&battery_estimate_sb, " | ");
    stringbuffer_append_ti (&battery_estimate_sb, remaining_secs);
    if (storage.last_full_timestamp != -1) {
        stringbuffer_append_str(&battery_estimate_sb, " | ");
        stringbuffer_append_ti (&battery_estimate_sb, time(NULL) - storage.last_full_timestamp);
    }

    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "battery_estimate_sb: %s", battery_estimate_sb.retval);
}

void battery_estimate_update(BatteryChargeState current) {
    LOG_FUNC();

    // if
    // - both current and former state are decharing
    // - the percentage droped by only one notch (which is 10%)
    // - the old state was stored in the last 24h
    // then
    // - advance write head, wrap around if necessary
    // - write a new time difference for a 10% drop
    if (!current.is_charging && !storage.battery_estimate.previous_state.is_charging &&
        !current.is_plugged && !storage.battery_estimate.previous_state.is_plugged      ) {
        if (current.charge_percent == (storage.battery_estimate.previous_state.charge_percent - 10)) {
            time_t current_timestamp = time(NULL);
            if (current_timestamp - 86400 < storage.battery_estimate.previous_state_timestamp) {
                storage.battery_estimate.average_data_write_head += 1;
                if (storage.battery_estimate.average_data_write_head >= battery_estimate_data_average_data_num) {
                    storage.battery_estimate.average_data_write_head = 0;
                }
                time_t difference = current_timestamp - storage.battery_estimate.previous_state_timestamp;
                storage.battery_estimate.averate_data[storage.battery_estimate.average_data_write_head] = difference;
                
                //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "battery_estimate.averate_data[%d] = %ld", storage.battery_estimate.average_data_write_head, storage.battery_estimate.averate_data[storage.battery_estimate.average_data_write_head]);
            }
        }
    }

    // store the timestamp the watch was last charged fully and unplugged.
    if ((storage.battery_estimate.previous_state.is_plugged && storage.battery_estimate.previous_state.charge_percent == 100) &&
        (!current.is_plugged && current.charge_percent == 100)) {
        storage.last_full_timestamp = time(NULL);
        //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "recording timestamp of full charge")
    }

    if (storage.battery_estimate.previous_state.is_charging    != current.is_charging ||
        storage.battery_estimate.previous_state.is_plugged     != current.is_plugged  ||
        storage.battery_estimate.previous_state.charge_percent != current.charge_percent) {
        LOG(LOG_BATTERY, "state has changed, recording new");
        memcpy(&(storage.battery_estimate.previous_state), &current, sizeof(BatteryChargeState));
        storage.battery_estimate.previous_state_timestamp = time(NULL);
        storage_persist();
    }

    battery_estimate_update_string(current);
}

char* battery_estimate_string(void) {
    LOG_FUNC();

    return battery_estimate_sb.retval;
}

