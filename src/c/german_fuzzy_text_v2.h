#pragma once

// Returns static buffer, which is overwritten with each call, containing a
// German text for the given hour and minute.
extern char* german_fuzzy_text_v2(int hour, int minute);
