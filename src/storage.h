#pragma once

#include <pebble.h>
#include "battery_estimate_data.h"

// The stored data.
typedef struct {
    char selectedVersion[64];
    battery_estimate_data battery_estimate;
    time_t last_full_timestamp;
} storage_t;
extern storage_t storage;

// Functions for setting up and tearing down the storage stuff. Also provides
// means to write back the settings.
void storage_init(void);
void storage_persist(void);
void storage_deinit(void);
