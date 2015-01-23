#include <pebble.h>
#include "storage.h"

// Indices to storage and data to access copies of stored values.
enum {
  SELECTED_VERISON = 0x0,
  BOTTOMSPACE_KEY = 0x1
};
storage_t storage;

// Makes sure storage is populated (by checking and, on absence, writing
// defaults) and then reading values.
void storage_init(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_init()");
    if (!persist_exists(BOTTOMSPACE_KEY)) {
        persist_write_int(BOTTOMSPACE_KEY, 3);
    }
    storage.bottomspace = persist_read_int(BOTTOMSPACE_KEY);
    if (!persist_exists(SELECTED_VERISON)) {
        persist_write_string(SELECTED_VERISON, "Regular");
    }
    persist_read_string(SELECTED_VERISON, storage.selectedVersion, 64);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.selectedVersion: %s", storage.selectedVersion);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.bottomspace: %d", storage.bottomspace);
}
    
// Write back values.
void storage_persist(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_persist()");
    persist_write_int(BOTTOMSPACE_KEY, storage.bottomspace);
    persist_write_string(SELECTED_VERISON, storage.selectedVersion);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.selectedVersion: %s", storage.selectedVersion);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage.bottomspace: %d", storage.bottomspace);
}

// Deinitalization, currently just writes back values but might do more in the
// future.
void storage_deinit(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "storage_deinit()");
    storage_persist();
}
