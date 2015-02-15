#include "battery_estimate_data.h"

const battery_estimate_data battery_estimate_data_init = {
    0,
    { 255, false, false },
    battery_estimate_data_average_data_num-1,
    { 43200, 43200, 43200, 43200, 43200, 43200, 43200, 43200, 43200, 43200 }
};
