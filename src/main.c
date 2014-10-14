#include <pebble.h>
#include "german_fuzzy_text.h"

// Static pointers for window and text layers.
static Window *s_main_window;
static TextLayer *s_german_text_layer;
static TextLayer *s_time_layer;
static TextLayer *s_bg_layer;

// Updates the text layers displaying the time, date and german text.
static void update_time() {
    // Get a tm structure from current time.
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    // Use german_fuzzy_text to render German text time and display in layer.
    char* buffer = german_fuzzy_text(tick_time->tm_hour, tick_time->tm_min);
    text_layer_set_text(s_german_text_layer, buffer);

    // Use a static (long lived) buffer for the numeric time.
    static char time[] = "00:00";

    // Write the current hours and minutes into the buffer, considerung the
    // 12/24h style.
    if(clock_is_24h_style() == true) {
      strftime(time, sizeof("00:00"), "%H:%M", tick_time);
    } else {
      strftime(time, sizeof("00:00"), "%I:%M", tick_time);
    }
    // Display time in respective layer.
    text_layer_set_text(s_time_layer, time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    // Hand updating to separate method, this method is used at watchface
    // loading as well.
    update_time();
}

static TextLayer *create_text_layer(GRect r, int fk) {
    // Create layer and 
    TextLayer *l = text_layer_create(r);
    text_layer_set_background_color(l, GColorClear);
    text_layer_set_text_color(l, GColorWhite);
    text_layer_set_font(l, fonts_get_system_font(fk));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(l));
    return l;
}

static void main_window_load(Window *window) {
    // Create blank, black background layer.
    s_bg_layer = text_layer_create(GRect(0, 0, 144, 168));
    text_layer_set_background_color(s_bg_layer, GColorBlack);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bg_layer));

    // 28 is the height of the lower time layer, this is used to bottom-align
    // the text.
    int16_t h = 28;

    // Create german text layer.
    s_german_text_layer = create_text_layer(GRect(0, 0, 144, 168-h), FONT_KEY_GOTHIC_28_BOLD);

    // Create time text layer and add it to window.
    s_time_layer = text_layer_create(GRect(0, 168-h, 144, h));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
    // Destroy text layers in reverse order.
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_german_text_layer);
    text_layer_destroy(s_bg_layer);
}

static void handle_init(void) {
    // Create window and add window handlers.
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers){
      .load = main_window_load,
      .unload = main_window_unload
    });

    // Push window onto stack and update text.
    window_stack_push(s_main_window, true);
    update_time();

    // Subscribe to timer tick, do this only here to not call time update
    // function twice.
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void handle_deinit(void) {
    // Destroy window.
    window_destroy(s_main_window);
}

int main(void) {
    handle_init();
    app_event_loop();
    handle_deinit();
}
