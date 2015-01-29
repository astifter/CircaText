#include <pebble.h>
#include "storage.h"

// Indices to storage and data to access copies of stored values.
enum {
  SELECTED_VERISON = 0x0,
  BOTTOMSPACE_KEY = 0x1
};
storage_t storage;

static void storage_log(void) {
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.selectedVersion: %s", storage.selectedVersion);
}

// Makes sure storage is populated (by checking and, on absence, writing
// defaults) and then reading values.
void storage_init(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init()");
    if (!persist_exists(SELECTED_VERISON)) {
        persist_write_string(SELECTED_VERISON, "Regular");
    }
    persist_read_string(SELECTED_VERISON, storage.selectedVersion, 64);

    storage_log();
}
    
// Write back values.
void storage_persist(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_persist()");
    persist_write_string(SELECTED_VERISON, storage.selectedVersion);

    storage_log();
}

// Deinitalization, currently just writes back values but might do more in the
// future.
void storage_deinit(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_deinit()");
    storage_persist();
}
