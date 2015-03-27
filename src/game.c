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
  
extern Window *game_window;
extern Layer *root_layer;

extern Window *main_window;
extern Layer *root_layer;
Layer *game_layer;

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

void init_game() {
    accel_data_service_subscribe(0, NULL);  // We will be using the accelerometer

  game_layer = root_layer;// create a new layer here //window_get_root_layer(window);
  
  layer_set_update_proc(game_layer, game_layer_update);
  load_graphics();
  init_board();
  init_muncher();
  create_players(0);
  currentplayer = 0;

  window_set_click_config_provider(main_window, game_click_config_provider);
}

void destroy_game() {
  accel_data_service_unsubscribe();
  //TODO: destroy game layer
  for (uint8_t i=0; i<4; i++) for (uint8_t j=0; j<4; j++) gbitmap_destroy(playersprite[i][j]);
  gbitmap_destroy(background);
}