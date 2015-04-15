/*
#include "global.h"
  
extern Window *main_window;
extern Layer *root_layer;

static Layer *intro_layer;
static AppTimer *intro_timer;

static uint16_t left=144+72;//144;

static void exit_intro() {
  if(intro_layer) layer_destroy(intro_layer);
  if(intro_timer) app_timer_cancel(intro_timer);
  mainmenu();
}

// ------------------------------------------------------------------------ //
//  Drawing
// ------------------------------------------------------------------------ //
static void blackout_framebuffer(uint8_t *screen, uint16_t right) {
#ifdef PBL_COLOR
  for(uint16_t y_addr=0, row=0; y_addr<168*144; y_addr+=144, row=(row+1)&7)
    for(uint16_t x_addr=0; x_addr<right; x_addr++)
      screen[y_addr+x_addr] = 0;
#else
  for(uint16_t y_addr=0, row=0; y_addr<168*20; y_addr+=20, row=(row+1)&7)
    for(uint16_t x_addr=0; x_addr<(right/8); x_addr++)
      screen[y_addr+x_addr] = 0;
#endif
}


static void introloop(void *data) {
  if(left==0)
    exit_intro();
  else {
    left -= (left>10) ? 10 : left;
    intro_timer = app_timer_register(UPDATE_MS, introloop, NULL);
    layer_mark_dirty(root_layer);  // Schedule redraw of screen
  }
}

static void intro_layer_update(Layer *me, GContext *ctx) {
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {
    uint8_t* screen = gbitmap_get_data(framebuffer);
    blackout_framebuffer(screen, (left>144)?144:left);
    graphics_release_frame_buffer(ctx, framebuffer);
  }
}

// ------------------------------------------------------------------------ //
//  Button Pushing
// ------------------------------------------------------------------------ //
static void click_handler(ClickRecognizerRef recognizer, void *context) {
  exit_intro();
}

static void bk_click_handler(ClickRecognizerRef recognizer, void *context) {
  window_stack_pop_all(false);
}

static void intro_button_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, bk_click_handler);
}

// ------------------------------------------------------------------------ //
//  Handling
// ------------------------------------------------------------------------ //
void create_intro_layer() {
  intro_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(intro_layer, intro_layer_update);
  layer_add_child(root_layer, intro_layer);
}

void intro() {
  create_pattern_layer();
  create_intro_layer();
  window_set_click_config_provider(main_window, intro_button_config);
  intro_timer = app_timer_register(UPDATE_MS, introloop, NULL);
}
*/