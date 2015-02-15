#include "battery_estimate.h"
#include "stringbuffer.h"
#include "battery_estimate_data.h"
#include "storage.h"
#include "logging_helper.h"

static stringbuffer battery_estimate_sb;
static int          battery_estimate_secs = -1;

void battery_estimate_init(void) {
    LOG_FUNC();

    stringbuffer_init(&battery_estimate_sb);
}

static void battery_estimate_update_string(void) {
    LOG_FUNC();

    stringbuffer_init(&battery_estimate_sb);
    if (storage.battery_display & battery_display_estimate) {
        stringbuffer_append_str(&battery_estimate_sb, " | ");
        if (battery_estimate_secs != -1) {
            stringbuffer_append_ti(&battery_estimate_sb, battery_estimate_secs);
        } else {
            stringbuffer_append(&battery_estimate_sb, "-");
        }
    }
    if (storage.battery_display & battery_display_runtime) {
        stringbuffer_append_str(&battery_estimate_sb, " | ");
        if (storage.last_full_timestamp != -1) {
            stringbuffer_append_ti(&battery_estimate_sb, time(NULL) - storage.last_full_timestamp);
        } else {
            stringbuffer_append(&battery_estimate_sb, "-");
        }
    }

    LOG_EXT(LOG_BATTERY, "battery_estimate_sb: %s", battery_estimate_sb.retval);
}

void battery_estimate_update(BatteryChargeState current) {
    LOG_FUNC();
    battery_estimate_data* be = &(storage.battery_estimate);
    BatteryChargeState* previous = &(be->previous_state);

    // if
    // - both current and former state are decharing
    // - the percentage droped by only one notch (which is 10%)
    // - the old state was stored in the last 24h
    // then
    // - advance write head, wrap around if necessary
    // - write a new time difference for a 10% drop
    int do_estimate_update = true;
    if (current.is_charging || previous->is_charging) {
        LOG(LOG_BATTERY, "not updating estimate, was or is charging");
        do_estimate_update = false;
    }
    if (current.is_plugged || previous->is_plugged) {
        LOG(LOG_BATTERY, "not updating estimate, was or is plugged");
        do_estimate_update = false;
    }
    if (current.charge_percent != (previous->charge_percent - 10)) {
        LOG(LOG_BATTERY, "not updating estimate, drop is not 10%");
        do_estimate_update = false;
    }
    time_t current_timestamp = time(NULL);
    if (current_timestamp - 86400 >= be->previous_state_timestamp) {
        LOG(LOG_BATTERY, "not updating estimate, pervious state is too old")
        do_estimate_update = false;
    }

    int needspersistence = false;
    // if estimation can be updated
    if (do_estimate_update) {
        LOG(LOG_BATTERY, "pervious and current state are valid, updating average data");
        be->average_data_write_head += 1;
        if (be->average_data_write_head >= battery_estimate_data_average_data_num) {
            be->average_data_write_head = 0;
        }

        time_t difference = current_timestamp - be->previous_state_timestamp;
        be->averate_data[be->average_data_write_head] = difference;
        needspersistence = true;
        
        LOG_EXT(LOG_BATTERY, "battery_estimate.averate_data[%d] = %ld", be->average_data_write_head, be->averate_data[be->average_data_write_head]);
    }

    // store the timestamp the watch was last charged fully and unplugged.
    if ((previous->is_plugged && previous->charge_percent == 100 && !previous->is_charging) &&
        (!current.is_plugged && current.charge_percent == 100                             )    ) {
        LOG(LOG_BATTERY, "recording timestamp of full charge")
        storage.last_full_timestamp = time(NULL);
        needspersistence = true;
    }

    // when the state has changed, record new state and timestamp.
    if (memcmp(previous, &current, sizeof(BatteryChargeState)) != 0) {
        LOG(LOG_BATTERY, "state has changed, recording new");
        memcpy(previous, &current, sizeof(BatteryChargeState));
        be->previous_state_timestamp = time(NULL);
        needspersistence = true;
    }
    if (needspersistence) storage_persist();

    unsigned int sum = 0;
    for (int i = 0; i < battery_estimate_data_average_data_num; i++) {
        sum += be->averate_data[i];
    }
    unsigned int average = sum / battery_estimate_data_average_data_num;
    unsigned int remaining = be->previous_state.charge_percent / 10;
    battery_estimate_secs = remaining * average;

    battery_estimate_update_string();
}

char* battery_estimate_string(void) {
    LOG_FUNC();
    battery_estimate_update_string();
    return battery_estimate_sb.retval;
}

