#include <pebble.h>
#include "text_handling.h"

// Handles creation of text layers.
TextLayer* create_text_layer(Window *window, GRect r, char *fk) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static TextLayer *create_text_layer(Window *window, GRect r, char *fk)");
    // Creates a text layer, sets proper colours, fonts and attaches it to
    // window.
    TextLayer *l = text_layer_create(r);
    text_layer_set_background_color(l, GColorClear);
    text_layer_set_text_color(l, GColorWhite);
    text_layer_set_font(l, fonts_get_system_font(fk));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(l));
    return l;
}

// Calculates the text height for a given font.
int16_t get_text_size(char *fk) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "int16_t get_text_size(char *fk)");
    return graphics_text_layout_get_content_size("00:00", fonts_get_system_font(fk),
                                                 GRect(0,0,100,100), 
                                                 GTextOverflowModeTrailingEllipsis,
                                                 GTextAlignmentLeft).h;
}
