#pragma once

#include <pebble.h>

typedef enum battery_estimate_data_e {
    battery_estimate_data_average_data_num = 11,
} battery_estimate_data_t;

typedef struct {
    time_t             previous_state_timestamp;
    BatteryChargeState previous_state;
    int                average_data_write_head;
    time_t             averate_data[battery_estimate_data_average_data_num];
} battery_estimate_data;

extern const battery_estimate_data battery_estimate_data_init;

void battery_estimate_data_log(time_t* data, size_t size, unsigned int how);
