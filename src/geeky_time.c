/*

   How to use a custom non-system font.

 */

#include "pebble.h"

static Window *window;

static TextLayer *time_layer;
static TextLayer *date_layer;
static TextLayer *temp_layer;

static BitmapLayer *icon_layer;
static BitmapLayer *therm_layer;
static GBitmap *icon_bitmap = NULL;
static GBitmap *therm_bitmap = NULL;

static const uint32_t WEATHER_ICONS[] = {
  RESOURCE_ID_IMG_SUNNY //0
};

static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {

  static char time_text[] = "00:00"; // Needs to be static because it's used by the system later.
  static char date_text[] = "Sun 01/01"; // Needs to be static because it's used by the system later.
  char *time_format;
  time_format = "%I:%M";
  if (clock_is_24h_style())
  {
    time_format = "%R";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Current time: %s", time_text);

  strftime(date_text, sizeof(date_text), "%a %m-%d", tick_time);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Current date: %s", date_text);

  text_layer_set_text(time_layer, time_text);
  text_layer_set_text(date_layer, date_text);
}



static void init() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Entering Init");
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);

  Layer *window_layer = window_get_root_layer(window);

  //TIME
  GFont custom_font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_42));
  time_layer = text_layer_create(GRect(2, 15, 144-2 /* width */, 45 /* 168 max height */));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, custom_font_time);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);

  layer_add_child(window_layer, text_layer_get_layer(time_layer));

  //DATE
  GFont custom_font_date = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_22));
  date_layer = text_layer_create(GRect(2, 65, 144-2 /* width */, 25 /* 168 max height */));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, custom_font_date);
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);

  layer_add_child(window_layer, text_layer_get_layer(date_layer));

  //THERM
  therm_layer = bitmap_layer_create(GRect(65, 112, 16, 36));
  layer_add_child(window_layer, bitmap_layer_get_layer(therm_layer));
  therm_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMG_THERM);
  bitmap_layer_set_bitmap(therm_layer, therm_bitmap);

  //TEMP
  GFont custom_font_temp = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TEMP_40));
  temp_layer = text_layer_create(GRect(81, 105, 144-85 /* width */, 60 /* 168 max height */));
  text_layer_set_text_alignment(temp_layer, GTextAlignmentCenter);
  text_layer_set_font(temp_layer, custom_font_temp);
  text_layer_set_background_color(temp_layer, GColorClear);
  text_layer_set_text_color(temp_layer, GColorWhite);

  layer_add_child(window_layer, text_layer_get_layer(temp_layer));
  

  icon_layer = bitmap_layer_create(GRect(5, 100, 60, 60));
  layer_add_child(window_layer, bitmap_layer_get_layer(icon_layer));


  icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[0]);
  bitmap_layer_set_bitmap(icon_layer, icon_bitmap);
   // text_layer_set_text(time_layer, "88:88");
   // text_layer_set_text(date_layer, "Sun 12:22");
  text_layer_set_text(temp_layer, "74");
  
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_minute_tick(current_time, MINUTE_UNIT);
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);

  
}

static void deinit() {
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(temp_layer);
  gbitmap_destroy(icon_bitmap);
  bitmap_layer_destroy(icon_layer);
  gbitmap_destroy(therm_bitmap);
  bitmap_layer_destroy(therm_layer);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}
