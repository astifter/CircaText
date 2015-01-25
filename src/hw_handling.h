#pragma once

#include <pebble.h>
#include "stringbuffer.h"

// The type of function to register as callback when hardware events have been
// processed.
typedef void (*hardware_changed_callback)(void);

// Functions for BlueTooth handling.
void bt_state_init(hardware_changed_callback c);
void bt_state_deinit(void);
char* get_bt_state(void);

// Functions for battery handling.
void battery_state_init(hardware_changed_callback c);
void battery_state_deinit(void);
char* get_battery_state(void);
