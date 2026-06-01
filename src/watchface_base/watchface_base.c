#include "watchface_base.h"
#include "storage.h"
#include "logging_helper.h"

void watchface_base_init(hardware_changed_callback func) {
    LOG_FUNC();

    storage_init();
    bt_state_init(func);
    battery_state_init(func);
}

void watchface_base_deinit(void) {
    LOG_FUNC();

    battery_state_deinit();
    bt_state_deinit();
    storage_deinit();
}
