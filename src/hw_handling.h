#pragma once

#include <pebble.h>
#include "stringbuffer.h"

// The type of function to register as callback when hardware events have been
// processed.
typedef void (*hardware_changed_callback)(void);

// Functions for BlueTooth handling.
void bt_state_init(hardware_changed_callback c);
void bt_state_deinit(void);
extern char* bt_state_string;
extern int   bt_state_string_dirty;

// Functions for battery handling.
void battery_state_init(hardware_changed_callback c);
void battery_state_deinit(void);
extern char* battery_state_string;
extern int   battery_state_string_dirty;
