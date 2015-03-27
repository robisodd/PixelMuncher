// ------------------------------------------------------------------------ //
//  Black and White Drawing Functions
// ------------------------------------------------------------------------ //
#ifdef PBL_BW
#include "render.h"
#include "player.h"
#include "muncher.h"
extern PlayerStruct player[5];
extern uint8_t current_player;

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
  //snprintf(text, sizeof(text), "Score:%ld Lives:%ld", (long int)get_score(get_current_player()), (long int)get_lives(get_current_player()));  // What text to draw
  snprintf(text, sizeof(text), "Score:%ld Lives:%ld", (long int)player[current_player].score, (long int)player[current_player].lives);  // What text to draw
//   snprintf(text, sizeof(text), "%d %d %ld %ld", accel.x, accel.y, atan2_lookup(accel.z, accel.x), atan2_lookup(-1 * accel.y, accel.z));  // What text to draw
  
  //snprintf(text, sizeof(text), "(%ld, %ld) %d", player[currentplayer].pos.x, player[currentplayer].pos.y, getmap(player[currentplayer].pos.x, player[currentplayer].pos.y));  // What text to draw
  //snprintf(text, sizeof(text), "123456789012345678901234");  // Max for "RESOURCE_ID_GOTHIC_14" and "RESOURCE_ID_GOTHIC_14_BOLD"
  graphics_draw_text(ctx, text, fonts_get_system_font("RESOURCE_ID_GOTHIC_14_BOLD"), GRect(0, -4, 144, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  // draw score (6 digits max -- 7 digits technically possible on level 100)
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void build_shadow_table() {}  // N/A in B&W

void fill_rect(uint8_t *screen, GRect rect, uint8_t color) {
  uint8_t data[] = {170, 85};
  if((color&192)!=0b00000000) {    // if not clear
    if(color==0b11000000)      {data[0]=  0; data[1]=  0;} // opaque black
    else if(color==0b11111111) {data[0]=255; data[1]=255;} // opaque white
    else                       {data[0]=170; data[1]= 85;} // opaque grey

    rect.size.w  += rect.origin.x; rect.size.h  += rect.origin.y;                      // convert rect.size.w and rect.size.h to rect.x2 and rect.y2
    rect.size.w   = rect.size.w   < 0 ? 0 : rect.size.w   > 144 ? 144 : rect.size.w;   // make sure rect.x2 is within screen bounds
    rect.origin.x = rect.origin.x < 0 ? 0 : rect.origin.x > 144 ? 144 : rect.origin.x; // make sure rect.x1 is within screen bounds
    rect.size.h   = rect.size.h   < 0 ? 0 : rect.size.h   > 168 ? 168 : rect.size.h;   // make sure rect.y2 is within screen bounds
    rect.origin.y = rect.origin.y < 0 ? 0 : rect.origin.y > 168 ? 168 : rect.origin.y; // make sure rect.y1 is within screen bounds

    GPoint addr;
    addr.y = rect.origin.y*20;
    uint8_t l_mask = 255 << (rect.origin.x%8); // mask for the left side
    uint8_t r_mask = 255 << (rect.size.w%8);   // mask for the right side

    for(int16_t y=0; y<(rect.size.h-rect.origin.y); y++, addr.y+=20) {
      addr.x = rect.origin.x>>3;       // init X memory address
      if  (addr.x >= 0 && addr.x < 19) screen[addr.y + addr.x] = (data[y&1] & l_mask) + (screen[addr.y + addr.x] & ~l_mask); // fill left-side of row
      for(addr.x++; addr.x<(rect.size.w>>3); addr.x++)
        if(addr.x >= 0 && addr.x < 19) screen[addr.y + addr.x] = data[y&1]; // fill middle of row
      if  (addr.x >= 0 && addr.x < 19) screen[addr.y + addr.x] = (screen[addr.y + addr.x] & r_mask) + (data[y&1] & ~r_mask); // fill right-side of row
    }
  }
}


#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------ //