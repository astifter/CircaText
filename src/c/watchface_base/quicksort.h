#pragma once

#if LOG_TESTING
#include <time.h>
#else
#include <pebble.h>
#endif

void quicksort(time_t* data, int first, int last);
