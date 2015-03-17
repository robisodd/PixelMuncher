// ------------------------------------------------------------------------ //
//  Black and White Drawing Functions
// ------------------------------------------------------------------------ //
#ifdef PBL_BW
#include "render.h"
#include "player.h"
#include "muncher.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void load_graphics() {
  background = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);

  playerspritesheet = gbitmap_create_with_resource(RESOURCE_ID_PLAYER_SPRITESHEET);
  for (uint8_t i=0; i<4; i++)
    for(uint8_t j=0; j<4;j++)
      playersprite[i][j] = gbitmap_create_as_sub_bitmap(playerspritesheet, GRect(i*7,j*7,7,7));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_background(GContext *ctx) {
  graphics_draw_bitmap_in_rect(ctx, background, GRect(0,13,144,155));
}
//void draw_background(GBitmap *fb) {
//  for (uint16_t i=0; i<BOARD_Y*5; i++) ((uint32_t*)(fb->addr))[i] = 0; // Top 13 rows of black -- equivilant to: graphics_context_set_fill_color(ctx, 0); graphics_fill_rect(ctx, GRect(0,0,144,12), 0, GCornerNone);
//  for (uint16_t i=0,j=BOARD_Y*5; i<155*5; i++,j++) ((uint32_t*)(fb->addr))[j] = ((uint32_t*)background->addr)[i]; // Draw Background Image -- equivilant to: graphics_draw_bitmap_in_rect(ctx, background, GRect(0,13,144,155));
//}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_dots(GContext *ctx) {
  extern int8_t map[];
  //totalpellets=0; 
  graphics_context_set_stroke_color(ctx, GColorWhite);
  if(((dotflashing>>2)&1)==0) 
    graphics_context_set_fill_color(ctx, GColorBlack);
  else
    graphics_context_set_fill_color(ctx, GColorWhite);
  //graphics_context_set_fill_color(ctx, (dotflashing>>2)&1);
  for(uint16_t y=0, i=0; y<MAP_H; y++)
    for(uint16_t x=0; x<MAP_W; x++, i++)
      if(map[i]==1) {
        //totalpellets++; 
        graphics_draw_pixel(ctx, GPoint(x*5+BOARD_X+2, y*5+BOARD_Y+2));
      } else if(map[i]==2) {
        //totalpellets++; 
        graphics_fill_rect(ctx, GRect(x*5+BOARD_X+1,y*5+BOARD_Y+1,3,3), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(x*5+BOARD_X+2,y*5+BOARD_Y,1,5), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(x*5+BOARD_X,y*5+BOARD_Y+2,5,1), 0, GCornerNone);
        //graphics_draw_pixel(ctx, GPoint(x*5+BOARD_X+2, y*5+BOARD_Y+2));
      }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_muncher(GContext *ctx) {
  extern MuncherStruct muncher;
  uint32_t *target;
  int32_t x, y;  // player's center dot on the screen
  graphics_context_set_stroke_color(ctx, GColorWhite);
  x = ((muncher.pos.x >> 6) * 5) + (((muncher.pos.x & 63) * 5) / 64);
  y = ((muncher.pos.y >> 6) * 5) + (((muncher.pos.y & 63) * 5) / 64);
  //target=(uint32_t*)playerspritesheet->addr + (((player[currentplayer].frame>>0)&3)*8);
  target = (uint32_t*)playerspritesheet + (((muncher.frame>>0)&3)*8) + 5;
  for(uint32_t i=0; i<7; i++) {
    for(uint32_t j=0; j<7; j++) {
        if((((*(target+i)) >> ((muncher.facing * 8) + j)) & 1) == 1)
          graphics_draw_pixel(ctx, GPoint(j+x+BOARD_X-3, i+y+BOARD_Y-3));
    }
  }

  //graphics_draw_bitmap_in_rect(ctx, playersprite[player[currentplayer].facing][(player[currentplayer].frame>>1)&1], GRect(x+BOARD_X-3, y+BOARD_Y-3, 7, 7));

//   graphics_context_set_stroke_color(ctx, dotflashing);
//   graphics_draw_pixel(ctx, GPoint(x+BOARD_X, y+BOARD_Y));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_spectres(GBitmap *fb) {
  
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_top(GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, 144, 13), 0, GCornerNone);
  
  static char text[40];  //Buffer to hold text
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  snprintf(text, sizeof(text), "Score:%ld Lives:%ld", (long int)get_score(get_current_player()), (long int)get_lives(get_current_player()));  // What text to draw
//   snprintf(text, sizeof(text), "%d %d %ld %ld", accel.x, accel.y, atan2_lookup(accel.z, accel.x), atan2_lookup(-1 * accel.y, accel.z));  // What text to draw
  
  //snprintf(text, sizeof(text), "(%ld, %ld) %d", player[currentplayer].pos.x, player[currentplayer].pos.y, getmap(player[currentplayer].pos.x, player[currentplayer].pos.y));  // What text to draw
  //snprintf(text, sizeof(text), "123456789012345678901234");  // Max for "RESOURCE_ID_GOTHIC_14" and "RESOURCE_ID_GOTHIC_14_BOLD"
  graphics_draw_text(ctx, text, fonts_get_system_font("RESOURCE_ID_GOTHIC_14_BOLD"), GRect(0, -4, 144, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  // draw score (6 digits max -- 7 digits technically possible on level 100)
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //


#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------ //