#pragma once

// Returns static buffer, which is overwritten with each call, containing a
// German text for the given hour and minute.
typedef char* (*tttp)(int hour, int minute);
