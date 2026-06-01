#pragma once

// Typedef for the function that returns a static character buffer for a given
// hour/minute combination.
typedef char* (*tttp)(int hour, int minute);
