#pragma once

#include <pebble.h>

TextLayer* create_text_layer(Window *window, GRect r, char *fk);
int16_t get_text_size(char *fk);
