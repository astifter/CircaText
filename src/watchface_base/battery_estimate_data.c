#include "battery_estimate_data.h"
#include "stringbuffer.h"
#include "logging_helper.h"

const battery_estimate_data battery_estimate_data_init = {
    0,
    { 255, false, false },
    battery_estimate_data_average_data_num-1,
    { 43052, 43082, 43112, 43142, 43172, 43202, 43232, 43262, 43292, 43322, 43352 }
};

void battery_estimate_data_log(time_t* data, size_t size, unsigned int how) {
    for (unsigned int i = 0; i < size; i++) {
        stringbuffer time;
        stringbuffer_init(&time);
        stringbuffer_append_ti(&time, data[i], time_format_full);
        LOG_EXT(how, "storage.battery_estimate.averate_data[%d]: %s", i, time.retval);
    }
}
