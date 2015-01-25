#include <pebble.h>
#include "stringbuffer.h"
#include "storage.h"
#include "appsync.h"
#include "hw_handling.h"
#include "main_window.h"

static Window      *s_main_window;
static TextLayer   *s_german_text_layer;
static TextLayer   *s_time_layer;
static TextLayer   *s_date_layer;
static BitmapLayer *s_bg_layer;
static TextLayer   *s_info1_layer;
static TextLayer   *s_info2_layer;

static bool ui_updates_enabled = false;

static const char* days[] = { "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So" };
static const char* months[] = { "Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" };

static void update_time_timer_callback(void* data) {
  //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "update_time()");
  update_time();
}

void flash_text(const char* text) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "void appsync_value_changed_callback(void)");
    if (!ui_updates_enabled) {
        //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): not done, not enabled");
        return;
    }

    light_enable_interaction();
    text_layer_set_text(s_german_text_layer, text);
  
    app_timer_register(4000, update_time_timer_callback, NULL);
}

// Updates the layers and writes all sorts of text.
void update_time(void) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time()");
    if (!ui_updates_enabled) {
        //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): not done, not enabled");
        return;
    }

    // Get a tm structure from current time.
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): fetch german text");
    // Use german_fuzzy_text to render German text time and display in layer.
    char* buffer = appsync_values.time_to_text_pointer(tick_time->tm_hour, tick_time->tm_min);
    text_layer_set_text(s_german_text_layer, buffer);

    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): display plain time");
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
    
    int weekday = tick_time->tm_wday-1;
    if (weekday < 0) weekday += 7;
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): display date");
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): day: %d", weekday);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): mday: %d", tick_time->tm_mday);
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): month: %d", tick_time->tm_mon);
    // Use a static (long lived) buffer for the numeric date.
    static char date[80];
    snprintf(date, 80, "%s, %d. %s", 
             days[weekday], 
             tick_time->tm_mday, 
             months[tick_time->tm_mon]
            );
    // Display date in respective layer.
    text_layer_set_text(s_date_layer, date);
    
    // Fetch and print BlueTooth status information.
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): display bt info");
    text_layer_set_text(s_info1_layer, get_bt_state());

    // Fetch and print battery status information.
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_time(): display battery info");
    text_layer_set_text(s_info2_layer, get_battery_state());
}

// Handles creation of text layers.
static TextLayer *create_text_layer(Window *window, GRect r, char *fk) {
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

// Updates the background bitmap layer.
static void update_bg_layer(struct Layer *layer, GContext *ctx) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void update_bg_layer(struct Layer *layer, GContext *ctx)");
    // First fill background with black rectangle.
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, layer_get_frame(layer), 0, 0);
    
    // Switch to white, fetch frame size.
    graphics_context_set_stroke_color(ctx, GColorWhite);
    GRect inforect = layer_get_frame(text_layer_get_layer(s_info1_layer));

    // Draw top and bottom line, use calculated data.
    uint16_t topline = inforect.origin.y + 3;
    graphics_draw_line(ctx, GPoint(inforect.origin.x, topline), 
                            GPoint(inforect.size.w,   topline));
    uint16_t bottomline = inforect.origin.y + inforect.size.h + 1;
    graphics_draw_line(ctx, GPoint(inforect.origin.x, bottomline), 
                            GPoint(inforect.size.w,   bottomline));
}

// Calculates the text height for a given font.
static int16_t get_text_size(char *fk) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "int16_t get_text_size(char *fk)");
    return graphics_text_layout_get_content_size("00:00", fonts_get_system_font(fk),
                                                 GRect(0,0,100,100), 
                                                 GTextOverflowModeTrailingEllipsis,
                                                 GTextAlignmentLeft).h;
}

// Loads and sets up main window.
static void main_window_load(Window *window) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void main_window_load(Window *window)");

    GRect screensize = layer_get_bounds(window_get_root_layer(window));
    GRect timerect, daterect, inforect;
    {
        int16_t h = get_text_size(FONT_KEY_GOTHIC_28);
        timerect = GRect(0, screensize.size.h-h-storage.bottomspace, screensize.size.w, h+storage.bottomspace);
    }
    {
        int16_t h = get_text_size(FONT_KEY_GOTHIC_24);
        daterect = GRect(0, screensize.size.h-h-storage.bottomspace, screensize.size.w, h+storage.bottomspace);
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

// Destroy allocated data.
static void main_window_unload(Window *window) {
    //app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "static void main_window_unload(Window *window)");

    // Destroy text layers in reverse order.
    text_layer_destroy(s_info2_layer);
    text_layer_destroy(s_info1_layer);
    text_layer_destroy(s_date_layer);
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_german_text_layer);
    bitmap_layer_destroy(s_bg_layer);
}

// Creates window and loads window handlers, pushes window onto display stack.
void main_window_create(void) {
    // Create window and add window handlers.
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers){
      .load = main_window_load,
      .unload = main_window_unload
    });

    // Push window onto stack and update text.
    window_stack_push(s_main_window, true);

    ui_updates_enabled = true;
    update_time();
}

void main_window_destroy(void) {
    window_destroy(s_main_window);
}