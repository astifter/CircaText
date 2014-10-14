#include <pebble.h>
#include "german_fuzzy_text.h"

static Window *s_main_window;
static TextLayer *s_german_text_layer;
static TextLayer *s_time_layer;
static TextLayer *s_bg_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  char* buffer = german_fuzzy_text(tick_time->tm_hour, tick_time->tm_min);
  text_layer_set_text(s_german_text_layer, buffer);
    
  // Create a long-lived buffer
  static char time[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(time, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(time, sizeof("00:00"), "%I:%M", tick_time);
  }
  // Display time in respective layer.
  text_layer_set_text(s_time_layer, time);
}
 
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
   update_time();    
}

static void main_window_load(Window *window) {
  s_bg_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(s_bg_layer, GColorBlack); 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bg_layer));
    
  int16_t h = 28;    

  // Create time TextLayer
  s_german_text_layer = text_layer_create(GRect(0, 0, 144, 168-h));
  text_layer_set_background_color(s_german_text_layer, GColorClear);
  text_layer_set_text_color(s_german_text_layer, GColorWhite);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_german_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_german_text_layer));
    

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 168-h, 144, h));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}
 
static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_german_text_layer);
    text_layer_destroy(s_bg_layer);
}

static void handle_init(void) {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
    
  window_stack_push(s_main_window, true);
    
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);  

  update_time();    
}

static void handle_deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
