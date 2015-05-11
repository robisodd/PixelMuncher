//56b37c6f-792a-480f-b962-9a0db8c32aa4
//b00bface-effd-480f-b962-c0ded4c0ffee
//#include "global.h"
#include "pebble.h"
void start_game();
void init_game();
void destroy_game();
void build_shadow_table();

Window *main_window;
Layer *root_layer;

// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void main_window_load(Window *window) {
  root_layer = window_get_root_layer(window);
  init_game();
  //intro();
  start_game();
//   initIntro will point root layer update proc to the intro (take black screen to )
//   initIntro(initgame(NULL));
//   or initgame(initmainmenu(NULL));
//   not sure which...
//   maybe not even NULL...
}
/*
*/
static void main_window_unload(Window *window) {
  destroy_game();
}

static void main_window_appear(Window *window) {
  APP_LOG(APP_MSG_OK, "A Wild Window Appeared!");
}    
static void main_window_disappear(Window *window) {
  APP_LOG(APP_MSG_OK, "The Main Window Ran Away!");
}

static void init(void) {
  // Set up and push main window
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
    .appear = main_window_appear,
    .disappear = main_window_disappear
  });
  #ifdef PBL_BW
    window_set_fullscreen(main_window, true);
  #endif

  //Set up other stuff
  srand(time(NULL));  // Seed randomizer
  accel_data_service_subscribe(0, NULL);  // We will be using the accelerometer
  //font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXEL_8));
  //build_shadow_table();
  
  //Begin
  window_stack_push(main_window, true); // Display window -- layer is now dirty.  Timer callback will be scheduled after dirty layer is written.  
}
  
static void deinit(void) {
  //unload_graphics()
  //fonts_unload_custom_font(font);
  accel_data_service_unsubscribe();
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}