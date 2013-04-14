/*
   ** SplitText Watch.
   Inspired by TextWatch
   With Date, 24H display and Week #
   Hour and Min has inversed background
   Pluses on new hours
 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "english_time.h"

#define MY_UUID { 0xE9, 0x9F, 0x3D, 0x76, 0xC4, 0x9A, 0x4E, 0x59, 0xA6, 0x38, 0xDB, 0xEB, 0xE1, 0x8E, 0x88, 0xD6 }
PBL_APP_INFO(MY_UUID,
             "SplitText", "atpeaz.com",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

TextLayer hourLayer;
TextLayer minLayer;
TextLayer dateLayer;
TextLayer day24weekLayer;

static char str_hour[20];
static char str_min[20];
static char str_date[30];
static char str_day24week[30];
static bool init_flag = false;

static void update_watch(PblTm* t) {
  
  char cur_hour[20];
  strcpy(cur_hour, str_hour);
  english_time(t->tm_hour, t->tm_min, str_hour, str_min);
  string_format_time(str_date, sizeof(str_date), "%e %B %Y", t);
  string_format_time(str_day24week, sizeof(str_day24week), "%A | %H%M | Week %W", t);
  
  if(strcmp(cur_hour,str_hour) != 0 && init_flag == true)
    vibes_short_pulse();

  text_layer_set_text(&hourLayer, str_hour);
  text_layer_set_text(&minLayer, str_min);
  text_layer_set_text(&dateLayer, str_date);
  text_layer_set_text(&day24weekLayer, str_day24week);
}

// Called once per minute
void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  update_watch(t->tick_time);  
}

// Handle the start-up of the app
void handle_init_app(AppContextRef app_ctx) {

  // Create our app's base window
  window_init(&window, "SplitText");
  window_stack_push(&window, true);
  window_set_background_color(&window, GColorBlack);

  // Init the text layers used to show the time

  // min
  text_layer_init(&minLayer, GRect(0, 56, 144, 168-56));
  text_layer_set_text_color(&minLayer, GColorWhite);
  text_layer_set_background_color(&minLayer, GColorClear);
  text_layer_set_font(&minLayer, fonts_get_system_font(FONT_KEY_GOTHAM_42_LIGHT));
  text_layer_set_text_alignment(&minLayer, GTextAlignmentLeft);
  layer_add_child(&window.layer, &minLayer.layer);

   // hour
  text_layer_init(&hourLayer, GRect(0, 16, 144, 44));
  text_layer_set_text_color(&hourLayer, GColorBlack);
  text_layer_set_background_color(&hourLayer, GColorWhite);
  text_layer_set_font(&hourLayer, fonts_get_system_font(FONT_KEY_GOTHAM_42_BOLD));
  text_layer_set_text_alignment(&hourLayer, GTextAlignmentLeft);
  layer_add_child(&window.layer, &hourLayer.layer);

  // date
  text_layer_init(&dateLayer, GRect(0, 0, 144, 18));
  text_layer_set_text_color(&dateLayer, GColorWhite);
  text_layer_set_background_color(&dateLayer, GColorClear);
  text_layer_set_font(&dateLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(&dateLayer, GTextAlignmentCenter);
  layer_add_child(&window.layer, &dateLayer.layer);

  // day24week
  text_layer_init(&day24weekLayer, GRect(0, 168-18, 144, 18));
  text_layer_set_text_color(&day24weekLayer, GColorBlack);
  text_layer_set_background_color(&day24weekLayer, GColorWhite);
  text_layer_set_font(&day24weekLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(&day24weekLayer, GTextAlignmentCenter);
  layer_add_child(&window.layer, &day24weekLayer.layer); 
  
  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  PblTm t;
  get_time(&t);
  update_watch(&t);

  init_flag = true;
}


// The main event/run loop for our app
void pbl_main(void *params) {
  PebbleAppHandlers handlers = {

    // Handle app start
    .init_handler = &handle_init_app,

    // Handle time updates
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
