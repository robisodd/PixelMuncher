#include <pebble.h>
#include "menu.h"
Layer *title_layer;
Layer *main_menu_layer;
Layer *options_menu_layer;
Layer *high_scores_layer;
#define shadowheight 4
extern Window *main_window;
extern Layer *root_layer;
AppTimer *menutimer;


static void menuloop(void *data) {
    menutimer = app_timer_register(UPDATE_MS, menuloop, NULL);
    layer_mark_dirty(root_layer);  // Schedule redraw of screen
}

// ------------------------------------------------------------------------ //
//  Drawing
// ------------------------------------------------------------------------ //
static void draw_frame(Layer *me, GContext *ctx) {
  //     me->clips = false;
  GRect rect;
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {   // if successfully captured the framebuffer
    uint8_t* screen = gbitmap_get_data(framebuffer);

    rect=me->frame;
    rect.origin.x += shadowheight;
    rect.origin.y += shadowheight;
    fill_rect(screen, rect, 0b10000000);

    rect=me->frame;
    #ifdef PBL_COLOR
      fill_rect(screen, rect, 0b10000011);
    #else
      fill_rect(screen, rect, 0b11000000);
    #endif
    graphics_release_frame_buffer(ctx, framebuffer);
  }  // endif successfully captured framebuffer

  graphics_context_set_stroke_color(ctx, GColorWhite);
  rect=me->frame;
  rect.origin.x = 1;
  rect.origin.y = 1;
  rect.size.h  -= 2;
  rect.size.w  -= 2;
//   graphics_draw_rect(ctx, rect);
}

static void draw_main_menu_layer(Layer *me, GContext *ctx) {
  
  draw_frame(me, ctx);
  GRect textframe;
  textframe = me->bounds;
  static char text[100];  //Buffer to hold text
  //textframe = GRect(11, 10, 120, 144);  // Text Box Position and Size: x, y, w, h
  snprintf(text, sizeof(text), "Options\n> Start Game!\nPlayers: 1\nHigh Scores");  // What text to draw
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), textframe, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text

  
}

static void options_menu_layer_update(Layer *me, GContext *ctx) {
  draw_frame(me, ctx);
}

static void title_layer_update(Layer *me, GContext *ctx) {
  draw_frame(me, ctx);
}

// ------------------------------------------------------------------------ //
//  Button Pushing
// ------------------------------------------------------------------------ //
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  main_menu_layer->bounds.origin.x += 5;
  main_menu_layer->frame.origin.x -= 10;
  main_menu_layer->frame.size.w += 20;
}
static void dn_click_handler(ClickRecognizerRef recognizer, void *context) {
  main_menu_layer->bounds.origin.x -= 5;
  main_menu_layer->frame.origin.x += 10;
  main_menu_layer->frame.size.w -= 20;
  
}
static void sl_click_handler(ClickRecognizerRef recognizer, void *context) {
 
}

static void bk_click_handler(ClickRecognizerRef recognizer, void *context) {
//   if(current_menu = main_menu)
    window_stack_pop_all(false);
  //else
    //switch_to(main_menu);
}

static void main_menu_button_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, dn_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, sl_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, bk_click_handler);
}


static void create_menu_layers() {
  title_layer = layer_create(GRect(22, 10, 100, 30));
  layer_set_update_proc(title_layer, title_layer_update);
  layer_add_child(root_layer, title_layer);

  main_menu_layer = layer_create(GRect(22, 50, 100, 100));
  layer_set_update_proc(main_menu_layer, draw_main_menu_layer);
  layer_add_child(root_layer, main_menu_layer);
  
  options_menu_layer = layer_create(GRect(22, 50, 100, 100));
  layer_set_update_proc(options_menu_layer, options_menu_layer_update);
  layer_add_child(root_layer, options_menu_layer);
  layer_set_hidden(options_menu_layer, true); // hidden


}

void mainmenu() {
  create_menu_layers();
  window_set_click_config_provider(main_window, main_menu_button_config);
  menutimer = app_timer_register(UPDATE_MS, menuloop, NULL);
}