#pragma once
#include "global.h"

  // defines e g #define something
  // typedef structs
GBitmap *background;
GBitmap *playerspritesheet,   *playersprite[4][4];
GBitmap *specturespritesheet, *spectresprite[4][4];
GBitmap *fruitspritesheet,    *bonussprite[1];


extern AccelData accel;
extern uint16_t totalpellets;
extern uint8_t dotflashing;
extern uint8_t speed;         // probably replace with level[currentlevel].playerspeed

void load_graphics();
void draw_background(GContext *ctx);
void draw_dots(GContext *ctx);
void draw_muncher(GContext *ctx);
void draw_top(GContext *ctx);

