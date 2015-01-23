#pragma once

#include <pebble.h>
#include "stringbuffer.h"

char* get_bt_state(void);
char* get_battery_state(void);

typedef void (*hardware_changed_callback)(void);

void bt_state_init(hardware_changed_callback c);
void bt_state_deinit(void);

void battery_state_init(hardware_changed_callback c);
void battery_state_deinit(void);
