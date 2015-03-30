// ------------------------------------------------------------------------ //
//  Color Drawing Functions
// ------------------------------------------------------------------------ //
#ifdef PBL_COLOR
#include "global.h"
extern PlayerStruct player[5];
extern uint8_t current_player;

//#ifdef PBL_PLATFORM_BASALT
//  s_fb_data = gbitmap_get_data(s_fb);
//  s_fb_size = gbitmap_get_bounds(s_fb).size;
//#else
//  s_fb_data = s_fb->addr;
//  s_fb_size = s_fb->bounds.size;
//#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
// Load game sprites  
void load_graphics() {
  background = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);

  playerspritesheet = gbitmap_create_with_resource(RESOURCE_ID_PLAYER_SPRITESHEET);
  for (uint8_t i=0; i<4; i++)
    for(uint8_t j=0; j<4;j++)
      playersprite[i][j] = gbitmap_create_as_sub_bitmap(playerspritesheet, GRect(i*7,j*7,7,7));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
//====================================//
// Background
//====================================//
void draw_background(GContext *ctx) {
  graphics_draw_bitmap_in_rect(ctx, background, GRect(0,13,144,155));
}
//void draw_background(GBitmap *fb) {
//  for (uint16_t i=0; i<BOARD_Y*5; i++) ((uint32_t*)(fb->addr))[i] = 0; // Top 13 rows of black -- equivilant to: graphics_context_set_fill_color(ctx, 0); graphics_fill_rect(ctx, GRect(0,0,144,12), 0, GCornerNone);
//  for (uint16_t i=0,j=BOARD_Y*5; i<155*5; i++,j++) ((uint32_t*)(fb->addr))[j] = ((uint32_t*)background->addr)[i]; // Draw Background Image -- equivilant to: graphics_draw_bitmap_in_rect(ctx, background, GRect(0,13,144,155));
//}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
//====================================//
// Dots
//====================================//
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
//====================================//
// Muncher
//====================================//
void draw_muncher(GContext *ctx) {
  extern MuncherStruct muncher;
  uint8_t *target;
  int32_t x, y;  // player's center dot on the screen
  graphics_context_set_stroke_color(ctx, GColorYellow);
  x = ((muncher.pos.x >> 6) * 5) + (((muncher.pos.x & 63) * 5) / 64);
  y = ((muncher.pos.y >> 6) * 5) + (((muncher.pos.y & 63) * 5) / 64);
  //target=(uint32_t*)playerspritesheet->addr + (((player[currentplayer].frame>>0)&3)*8);
  //target = (uint8_t*)playerspritesheet+(32*0);// + (((muncher.frame>>0)&3)*8) + 5;
target=(uint8_t*)gbitmap_get_data(playerspritesheet);
/*
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, 144, 168), 0, GCornerNone);
  static char text[400];  //Buffer to hold text
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  snprintf(text, sizeof(text), "%x\n%x\n%x\n%x\n%x\n%x\n%x\n%x\n", *(target+0), *(target+1), *(target+2), *(target+3), *(target+4), *(target+5), *(target+6), *(target+7));  // What text to draw
  graphics_draw_text(ctx, text, fonts_get_system_font("RESOURCE_ID_GOTHIC_14"), GRect(0, -4, 144, 172), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
*/

  graphics_context_set_stroke_color(ctx, GColorYellow);
  for(uint32_t i=0; i<7; i++) {
    for(uint32_t j=0; j<7; j++) {
      if(*(target + ((i+(muncher.frame*8))*32) + (muncher.facing*8) + j) == 255)
        graphics_draw_pixel(ctx, GPoint(j+x+BOARD_X-3, i+y+BOARD_Y-3));
    //if((((*(target+i)) >> ((muncher.facing * 8) + j)) & 1) == 1)      
    }
  }
  
/*
  for(uint32_t i=0; i<7; i++) {
    for(uint32_t j=0; j<7; j++) {
        //if(*(target+(i*32)+j) != 192)
     graphics_context_set_stroke_color(ctx, GColorYellow);
    if((((*(target+i)) >> ((muncher.facing * 8) + j)) & 1) == 1)      
      graphics_draw_pixel(ctx, GPoint(j+x+BOARD_X-3, i+y+BOARD_Y-3));
      //else
    }
  }

*/
  
  
  /*
  for(uint32_t i=0; i<32; i++) {
    for(uint32_t j=0; j<32; j++) {
      //if(*(target+(i*32)+j) != 192)
      //graphics_context_set_stroke_color(ctx, (GColor)(*(target+(i*32)+j)));
      graphics_context_set_stroke_color(ctx, (GColor){.argb=(192+(  ((*(target+(i*32)+j)))&64) )});
      graphics_draw_pixel(ctx, GPoint(j+x+BOARD_X-3, i+y+BOARD_Y-3));
    }
    }
  
  for(uint32_t i=0; i<7; i++) {
    for(uint32_t j=0; j<7; j++) {
        //if(*(target+(i*32)+j) != 192)
     graphics_context_set_stroke_color(ctx, (GColor)(*(target+(i*32)+j)));
      
      graphics_draw_pixel(ctx, GPoint(j+x+BOARD_X-3, i+y+BOARD_Y-3));
      //else
        //if((((*(target+i)) >> ((muncher.facing * 8) + j)) & 1) != 192)
    }
  }
*/
      
  //graphics_draw_bitmap_in_rect(ctx, playersprite[player[currentplayer].facing][(player[currentplayer].frame>>1)&1], GRect(x+BOARD_X-3, y+BOARD_Y-3, 7, 7));

//   graphics_context_set_stroke_color(ctx, dotflashing);
//   graphics_draw_pixel(ctx, GPoint(x+BOARD_X, y+BOARD_Y));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_spectres(GBitmap *fb) {
  
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_top(GContext *ctx) {
  // Top means the top 13 pixels, including score, lives, current player, level and bonus
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, 144, 13), 0, GCornerNone);
  
  static char text[40];  //Buffer to hold text
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  snprintf(text, sizeof(text), "Score:%ld Lives:%ld", (long int)player[current_player].score, (long int)player[current_player].lives);  // What text to draw
//   snprintf(text, sizeof(text), "%d %d %ld %ld", accel.x, accel.y, atan2_lookup(accel.z, accel.x), atan2_lookup(-1 * accel.y, accel.z));  // What text to draw
  
  //snprintf(text, sizeof(text), "(%ld, %ld) %d", player[currentplayer].pos.x, player[currentplayer].pos.y, getmap(player[currentplayer].pos.x, player[currentplayer].pos.y));  // What text to draw
  //snprintf(text, sizeof(text), "123456789012345678901234");  // Max for "RESOURCE_ID_GOTHIC_14" and "RESOURCE_ID_GOTHIC_14_BOLD"
  graphics_draw_text(ctx, text, fonts_get_system_font("RESOURCE_ID_GOTHIC_14_BOLD"), GRect(0, -4, 144, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  // draw score (6 digits max -- 7 digits technically possible on level 100)
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
uint8_t shadowtable[256];
void build_shadow_table() {
  uint8_t r, g, b;
  for(uint8_t i=0; i<64; i++) {
    shadowtable[0b11000000+i] = 0b11000000+i;     // 100% None Shade (Same Color)
    r = i & 0b00110000;
    g = i & 0b00001100;
    b = i & 0b00000011;
    if(r>0) r-=0b00010000;
    if(g>0) g-=0b00000100;
    if(b>0) b-=0b00000001;
    shadowtable[0b10000000+i] = 0b11000000+r+g+b; //  66% Some Shade (Slightly Darker)
    if(r>0) r-=0b00010000;
    if(g>0) g-=0b00000100;
    if(b>0) b-=0b00000001;
    shadowtable[0b01000000+i] = 0b11000000+r+g+b; //  33% Much Shade (Really Dark)
    shadowtable[0b00000000+i] = 0b11000000;       //   0% Full Shade (Completely Dark)
  }
}

void fill_rect(uint8_t *screen, GRect rect, uint8_t color) {
  uint8_t bg_opacity = (~color)&0b11000000;
  rect.size.w  += rect.origin.x; rect.size.h  += rect.origin.y;                      // convert rect.size.w and rect.size.h to rect.x2 and rect.y2
  rect.size.w   = rect.size.w   < 0 ? 0 : rect.size.w   > 144 ? 144 : rect.size.w;   // make sure rect.x2 is within screen bounds
  rect.origin.x = rect.origin.x < 0 ? 0 : rect.origin.x > 144 ? 144 : rect.origin.x; // make sure rect.x1 is within screen bounds
  rect.size.h   = rect.size.h   < 0 ? 0 : rect.size.h   > 168 ? 168 : rect.size.h;   // make sure rect.y2 is within screen bounds
  rect.origin.y = rect.origin.y < 0 ? 0 : rect.origin.y > 168 ? 168 : rect.origin.y; // make sure rect.y1 is within screen bounds

  rect.origin.y*=144; rect.size.h*=144;
  for (uint16_t y_addr=rect.origin.y; y_addr<rect.size.h; y_addr+=144)
    for(uint16_t x_addr=rect.origin.x; x_addr<rect.size.w; x_addr++)
      screen[y_addr+x_addr] = shadowtable[bg_opacity + (screen[y_addr+x_addr]&63)] + color;
}

#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------ //