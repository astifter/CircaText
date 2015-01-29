#include "battery_estimate.h"
#include "stringbuffer.h"
#include "battery_estimate_data.h"
#include "storage.h"
#include "logging_helper.h"

static stringbuffer battery_estimate_sb;

void battery_estimate_init(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void battery_estimate_init(void)");

    stringbuffer_init(&battery_estimate_sb);
}

static void battery_estimate_update_string(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void battery_estimate_update_string(void)");

    unsigned int sum = 0;
    for (int i = 0; i < battery_estimate_data_average_data_num; i++) {
        sum += storage.battery_estimate.averate_data[i];
    }
    unsigned int average = sum / battery_estimate_data_average_data_num;
    unsigned int remaining = storage.battery_estimate.previous_state.charge_percent / 10;
    unsigned int remaining_secs = remaining * average;

    unsigned int seconds = remaining_secs % 60; remaining_secs /= 60;
    unsigned int minutes = remaining_secs % 60; remaining_secs /= 60;
    unsigned int hours   = remaining_secs % 24; remaining_secs /= 24;
    unsigned int days    = remaining_secs;

    if (days == 0 && hours == 0) {
        stringbuffer_append_fi(&battery_estimate_sb, "%dm", minutes);
        stringbuffer_append_fi(&battery_estimate_sb, "%ds", seconds);
    } else if (days == 0) {
        stringbuffer_append_fi(&battery_estimate_sb, "%dh", hours);
        stringbuffer_append_fi(&battery_estimate_sb, "%dm", minutes);
    } else {
        stringbuffer_append_fi(&battery_estimate_sb, "%dd", days);
        stringbuffer_append_fi(&battery_estimate_sb, "%dh", hours);
    }
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "battery_estimate_sb: %s", battery_estimate_sb.retval);
}

void battery_estimate_update(BatteryChargeState current) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void battery_estimate_update(BatteryChargeState current)");

    memcpy(&(storage.battery_estimate.previous_state), &current, sizeof(BatteryChargeState));
    storage.battery_estimate.previous_state_timestamp = time(NULL);
    storage_persist();

    battery_estimate_update_string();
}

char* battery_estimate_string(void) {
    return battery_estimate_sb.retval;
}

