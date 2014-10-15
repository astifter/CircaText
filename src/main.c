#include <pebble.h>
#include "german_fuzzy_text.h"

// Static pointers for window and text layers.
static Window *s_main_window;
static TextLayer *s_german_text_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_bg_layer;
static TextLayer *s_info_layer;

static bool bt_connection;
static BatteryChargeState battery_state;

static char* days[] = { "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So" };
static char* months[] = { "Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" };

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
    
    // Use a static (long lived) buffer for the numeric date.
    static char date[20];
    snprintf(date, 19, "%s, %d. %s", 
             days[tick_time->tm_wday-1], 
             tick_time->tm_mday, 
             months[tick_time->tm_mon]
            );
    // Display date in respective layer.
    text_layer_set_text(s_date_layer, date);
    
    static char info[80];
    char* batterystatestring;
    if (!battery_state.is_plugged) batterystatestring = "";
    else if (battery_state.is_charging) batterystatestring = " (p, c)";
    else batterystatestring = " (p)";
    snprintf(info, 79, "bt: %s -- batt: %d%%%s", bt_connection ? "c" : "d", 
                                                 battery_state.charge_percent, 
                                                 batterystatestring);
    text_layer_set_text(s_info_layer, info);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    // Hand updating to separate method, this method is used at watchface
    // loading as well.
    update_time();
}

static TextLayer *create_text_layer(Window *window, GRect r, char *fk) {
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
    int16_t h = 31;

    // Create german text layer.
    s_german_text_layer = create_text_layer(window, GRect(0, 0, 144, 168-h), FONT_KEY_GOTHIC_28_BOLD);

    // Create time text layer and add it to window.
    s_time_layer = create_text_layer(window, GRect(0, 168-h, 144, h), FONT_KEY_GOTHIC_28);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
    
    // Create date text layer and add it to window.
    h = 27;
    s_date_layer = create_text_layer(window, GRect(0, 168-h, 144, h), FONT_KEY_GOTHIC_24);
    
    // Create information layer and add it to the window.
    s_info_layer = create_text_layer(window, GRect(0, 168-33-15, 144, 15), FONT_KEY_GOTHIC_14);
    text_layer_set_overflow_mode(s_info_layer, GTextOverflowModeTrailingEllipsis);
}

static void main_window_unload(Window *window) {
    // Destroy text layers in reverse order.
    text_layer_destroy(s_info_layer);
    text_layer_destroy(s_date_layer);
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_german_text_layer);
    text_layer_destroy(s_bg_layer);
}

void handle_bt_event(bool connected) {
    bt_connection = connected;
    update_time();
}

void handle_battery_event(BatteryChargeState s) {
    battery_state = s;
    update_time();
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

    bt_connection = bluetooth_connection_service_peek();
    bluetooth_connection_service_subscribe(handle_bt_event);
    battery_state = battery_state_service_peek();
    battery_state_service_subscribe(handle_battery_event);

    update_time();
    // Subscribe to timer tick, do this only here to not call time update
    // function twice.
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void handle_deinit(void) {
    tick_timer_service_unsubscribe();
    battery_state_service_unsubscribe();
    bluetooth_connection_service_unsubscribe();
    // Destroy window.
    window_destroy(s_main_window);    
}

int main(void) {
    handle_init();
    app_event_loop();
    handle_deinit();
}
