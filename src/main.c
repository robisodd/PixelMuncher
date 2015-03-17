//56b37c6f-792a-480f-b962-9a0db8c32aa4
//b00bface-effd-480f-b962-c0ded4c0ffee
#include "global.h"
#include "player.h"
#include "muncher.h"
#include "spectre.h"
#include "map.h"
#include "input.h"
#include "render.h"
#include "game.h"

//uint16_t totalpellets;  // delete this, not needed anymore
  
static Window *game_window;
static Layer *game_layer;

PlayerStruct player[1];
uint8_t currentplayer = 0;
uint8_t dotflashing=0;




//Timer
//#define UPDATE_MS 50 // Refresh rate in milliseconds
//static AppTimer *timer;
//timer = app_timer_register(UPDATE_MS, timer_callback, NULL); // Schedule a callback, save handle in timer



//AccelData accel;// = (AccelData) { .x = 0, .y = 0, .z = 0 };
// ------------------------------------------------------------------------ //
//  Main Loop Functions
// ------------------------------------------------------------------------ //
static void gameloop(void *data) {
  extern MuncherStruct muncher;
  muncher.speed=getlevelspeed();

  muncher_eat_dots();
  update_movement_via_joystick();
  move_muncher();
  dotflashing++;
  
  layer_mark_dirty(game_layer);  // Schedule redraw of screen
}




//====================================//
static void game_layer_update(Layer *me, GContext *ctx) {
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {
    //draw_background(framebuffer);
    graphics_release_frame_buffer(ctx, framebuffer);
  }
  draw_background(ctx);
  draw_dots(ctx);
  draw_muncher(ctx);
  draw_top(ctx);

  app_timer_register(UPDATE_MS, gameloop, NULL); // Finished. Wait UPDATE_MS then loop
}
  
// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void game_window_load(Window *window) {
  game_layer = window_get_root_layer(window);
  layer_set_update_proc(game_layer, game_layer_update);
  load_graphics();
  init_board();
  init_muncher();
  create_players(0);
  currentplayer = 0;
  
}

static void game_window_unload(Window *window) {
//   for (uint16_t i=0; i<9; i++) gbitmap_destroy(playersprite[i]);
  for (uint8_t i=0; i<4; i++) for (uint8_t j=0; j<4; j++) gbitmap_destroy(playersprite[i][j]);
  gbitmap_destroy(background);
}

static void game_window_appear(Window *window) {}
static void game_window_disappear(Window *window) {}

static void init(void) {
  // Set up and push main window
  game_window = window_create();
  window_set_click_config_provider(game_window, game_click_config_provider);
  window_set_window_handlers(game_window, (WindowHandlers) {
    .load = game_window_load,
    .unload = game_window_unload,
    .appear = game_window_appear,
    .disappear = game_window_disappear
  });
  window_set_fullscreen(game_window, true);

  //Set up other stuff
  srand(time(NULL));  // Seed randomizer
  accel_data_service_subscribe(0, NULL);  // We will be using the accelerometer
  //font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXEL_8));
  
  //Begin
  window_stack_push(game_window, true /* Animated */); // Display window (layer is now dirty).  Timer callback will be scheduled after dirty layer is written.
}
  
static void deinit(void) {
  //fonts_unload_custom_font(font);
  accel_data_service_unsubscribe();
  window_destroy(game_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

// ------------------------------------------------------------------------ //
//  Notes
// ------------------------------------------------------------------------ //
/*
#define FONT_KEY_FONT_FALLBACK "RESOURCE_ID_FONT_FALLBACK"
#define FONT_KEY_GOTHIC_18 "RESOURCE_ID_GOTHIC_18"
#define FONT_KEY_GOTHIC_18_BOLD "RESOURCE_ID_GOTHIC_18_BOLD"
#define FONT_KEY_GOTHIC_24 "RESOURCE_ID_GOTHIC_24"
#define FONT_KEY_GOTHIC_24_BOLD "RESOURCE_ID_GOTHIC_24_BOLD"
#define FONT_KEY_GOTHIC_28 "RESOURCE_ID_GOTHIC_28"
#define FONT_KEY_GOTHIC_28_BOLD "RESOURCE_ID_GOTHIC_28_BOLD"
#define FONT_KEY_BITHAM_30_BLACK "RESOURCE_ID_BITHAM_30_BLACK"
#define FONT_KEY_BITHAM_42_BOLD "RESOURCE_ID_BITHAM_42_BOLD"
#define FONT_KEY_BITHAM_42_LIGHT "RESOURCE_ID_BITHAM_42_LIGHT"
#define FONT_KEY_BITHAM_42_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_42_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_34_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_LIGHT_SUBSET "RESOURCE_ID_BITHAM_34_LIGHT_SUBSET"
#define FONT_KEY_BITHAM_18_LIGHT_SUBSET "RESOURCE_ID_BITHAM_18_LIGHT_SUBSET"
#define FONT_KEY_ROBOTO_CONDENSED_21 "RESOURCE_ID_ROBOTO_CONDENSED_21"
#define FONT_KEY_ROBOTO_BOLD_SUBSET_49 "RESOURCE_ID_ROBOTO_BOLD_SUBSET_49"
#define FONT_KEY_DROID_SERIF_28_BOLD "RESOURCE_ID_DROID_SERIF_28_BOLD"





*/