#pragma once

#include <pebble.h>
#include "stringbuffer.h"

// The type of function to register as callback when hardware events have been
// processed.
typedef void (*hardware_changed_callback)(void);

// Initializers and Deinitializers.
void bt_state_init(hardware_changed_callback c);
void bt_state_deinit(void);
void battery_state_init(hardware_changed_callback c);
void battery_state_deinit(void);

// Functions to fetch the current state of the HW as descriptive strings.
char* get_bt_state(void);
char* get_battery_state(void);
