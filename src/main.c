#include <pebble.h>
#include "german_fuzzy_text.h"

// Static pointers for window and text layers.
static Window *s_main_window;
static TextLayer *s_german_text_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static BitmapLayer *s_bg_layer;
static TextLayer *s_info1_layer;
static TextLayer *s_info2_layer;

#define BOTTOMSPACE_KEY 1
static int bottomspace;

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
    
    static char bt_info[80];
    char *statestring;
    if (bt_connection) statestring = "conn"; else statestring = "disconn";
    snprintf(bt_info, 79, "bt: %s", statestring);
    text_layer_set_text(s_info1_layer, bt_info);

    static char batt_info[80];
    char* batterystatestring;
    if (!battery_state.is_plugged) batterystatestring = "";
    else if (battery_state.is_charging) batterystatestring = " (p, c)";
    else batterystatestring = " (p)";
    snprintf(batt_info, 79, "batt: %d%%%s", battery_state.charge_percent, 
                                            batterystatestring);
    text_layer_set_text(s_info2_layer, batt_info);
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

static void update_bg_layer(struct Layer *layer, GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, layer_get_frame(layer), 0, 0);
    
    graphics_context_set_stroke_color(ctx, GColorWhite);
    GRect inforect = layer_get_frame(text_layer_get_layer(s_info1_layer));
    uint16_t topline = inforect.origin.y + 3;
    graphics_draw_line(ctx, GPoint(inforect.origin.x, topline), 
                            GPoint(inforect.size.w,   topline));
    uint16_t bottomline = inforect.origin.y + inforect.size.h + 1;
    graphics_draw_line(ctx, GPoint(inforect.origin.x, bottomline), 
                            GPoint(inforect.size.w,   bottomline));
}

int16_t get_text_size(char *fk) {
    return graphics_text_layout_get_content_size("00:00", fonts_get_system_font(fk),
                                                 GRect(0,0,100,100), 
                                                 GTextOverflowModeTrailingEllipsis,
                                                 GTextAlignmentLeft).h;
}

static void main_window_load(Window *window) {
    GRect screensize = layer_get_bounds(window_get_root_layer(window));
    GRect timerect, daterect, inforect;
    {
        int16_t h = get_text_size(FONT_KEY_GOTHIC_28);
        timerect = GRect(0, screensize.size.h-h-bottomspace, screensize.size.w, h+bottomspace);
    }
    {
        int16_t h = get_text_size(FONT_KEY_GOTHIC_24);
        daterect = GRect(0, screensize.size.h-h-bottomspace, screensize.size.w, h+bottomspace);
    }
    {
        int16_t h = get_text_size(FONT_KEY_GOTHIC_14);
        inforect = GRect(0, daterect.origin.y-h, screensize.size.w, h);
    }
    
    // Create blank, black background layer.
    s_bg_layer = bitmap_layer_create(screensize);
    layer_set_update_proc(bitmap_layer_get_layer(s_bg_layer), update_bg_layer);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bg_layer));

    // Create german text layer.
    s_german_text_layer = create_text_layer(window, screensize, FONT_KEY_GOTHIC_28_BOLD);

    // Create time text layer and add it to window.
    s_time_layer = create_text_layer(window, timerect, FONT_KEY_GOTHIC_28);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
    
    // Create date text layer and add it to window.
    s_date_layer = create_text_layer(window, daterect, FONT_KEY_GOTHIC_24);
    
    // Create information layer and add it to the window.
    s_info1_layer = create_text_layer(window, inforect, FONT_KEY_GOTHIC_14);
    text_layer_set_overflow_mode(s_info1_layer, GTextOverflowModeTrailingEllipsis);
    s_info2_layer = create_text_layer(window, inforect, FONT_KEY_GOTHIC_14);
    text_layer_set_overflow_mode(s_info2_layer, GTextOverflowModeTrailingEllipsis);
    text_layer_set_text_alignment(s_info2_layer, GTextAlignmentRight);
}

static void main_window_unload(Window *window) {
    // Destroy text layers in reverse order.
    text_layer_destroy(s_info2_layer);
    text_layer_destroy(s_info1_layer);
    text_layer_destroy(s_date_layer);
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_german_text_layer);
    bitmap_layer_destroy(s_bg_layer);
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
    // Persistent storage.
    if (persist_exists(BOTTOMSPACE_KEY)) {
        bottomspace = persist_read_int(BOTTOMSPACE_KEY);
        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "exisiting BOTTOMSPACE_KEY: %d", bottomspace);
    } else {
        bottomspace = 3;
        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "new BOTTOMSPACE_KEY: %d", bottomspace);
    }
    
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
    persist_write_int(BOTTOMSPACE_KEY, bottomspace);
    
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
