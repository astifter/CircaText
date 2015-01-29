#pragma once

#include <pebble.h>

void battery_estimate_init(void);
void battery_estimate_unlock(void);
void battery_estimate_update(BatteryChargeState current);
char* battery_estimate_string(void);
