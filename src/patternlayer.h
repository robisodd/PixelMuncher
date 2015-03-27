#pragma once
#include "global.h"
#include "render.h"
  
void set_pattern(uint8_t *data);
void fill_framebuffer_with_pattern(uint8_t *screen, uint8_t *data);
void modify_pattern(uint8_t *data, int8_t x_offset, int8_t y_offset, uint8_t invert);
void create_pattern_layer();
void destroy_pattern_layer();