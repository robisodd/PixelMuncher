#include <pebble.h>
#include "patternlayer.h"
  
extern Window *main_window;
extern Layer *root_layer;
Layer *pattern_layer;

static uint8_t *pattern;
extern uint8_t brickpattern[];

void set_pattern(uint8_t *data) {
  pattern = data;
}

void modify_pattern(uint8_t *data, int8_t x_offset, int8_t y_offset, uint8_t invert) {  // invert = [0 or 255] for [no or yes] ([1-254] inverts vertical stripes based on bit mask)
  uint8_t temp[10];
  uint8_t mask = ((x_offset%8)+8)%8;  // sanatize x_offset [-128 to 127] to [0 to  7]
  uint8_t row  = ((y_offset%8)+8);    // sanatize y_offset [-128 to 127] to [0 to 15]
  for(uint8_t i=0; i<8; i++)
    temp[i] = ((data[(row+i)%8] << mask) + (data[(row+i)%8] >> (8-mask))) ^ invert; // shift 8bit segment x&y offset, then invert using XOR
  for(uint8_t i=0; i<8; i++) data[i] = temp[i];
}

void fill_framebuffer_with_pattern(uint8_t *screen, uint8_t *data) {
#ifdef PBL_COLOR
  for(uint16_t y_addr=0, row=0; y_addr<168*144; y_addr+=144, row=(row+1)&7)
    for(uint16_t x_addr=0; x_addr<144; x_addr++)
      screen[y_addr+x_addr] = ((data[row]>>(x_addr&7))&1) ? data[8] : data[9]; //colorwhite : colorblack;
#else
  for(uint16_t y_addr=0, row=0; y_addr<168*20; y_addr+=20, row=(row+1)&7)
    for(uint16_t x_addr=0; x_addr<19; x_addr++)
      screen[y_addr+x_addr] = data[row];
#endif
}

static void pattern_layer_update(Layer *me, GContext *ctx) {
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {
    uint8_t* screen = gbitmap_get_data(framebuffer);
    fill_framebuffer_with_pattern(screen, pattern);
    graphics_release_frame_buffer(ctx, framebuffer);
  }
  modify_pattern(brickpattern, -2, 0, 0);
}

// static void move_pattern(void *data) {
//   modify_pattern(brickpattern, -2, 0, 0);
//   patterntimer = app_timer_register(50, move_pattern, NULL);
// }

void create_pattern_layer() {
  pattern_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(pattern_layer, pattern_layer_update);
  layer_add_child(root_layer, pattern_layer);
  pattern=brickpattern;
//   patterntimer = app_timer_register(50, move_pattern, NULL);
}

void destroy_pattern_layer() {
  layer_destroy(pattern_layer);
}