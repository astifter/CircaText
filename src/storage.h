#pragma once

#include <pebble.h>

// The stored data.
typedef struct {
    char selectedVersion[64];
    int bottomspace;
} storage_t;
extern storage_t storage;

// Functions for setting up and tearing down the storage stuff. Also provides
// means to write back the settings.
void storage_init(void);
void storage_persist(void);
void storage_deinit(void);
