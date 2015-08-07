#include "global.h"

GBitmap *font8_bmp; uint8_t *font8;
GBitmap *background_bmp; uint8_t *background;
GBitmap *font_sprites_bmp; uint8_t *font_sprites;

// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void load_graphics() {
          font8_bmp = gbitmap_create_with_resource(RESOURCE_ID_FONT8);
     background_bmp = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
   font_sprites_bmp = gbitmap_create_with_resource(RESOURCE_ID_FONT_SPRITES);

              font8 = gbitmap_get_data(font8_bmp);
         background = gbitmap_get_data(background_bmp);
       font_sprites = gbitmap_get_data(font_sprites_bmp);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void unload_graphics() {
  gbitmap_destroy(font8_bmp);
  gbitmap_destroy(background_bmp);
  gbitmap_destroy(font_sprites_bmp);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_background_ctx(GContext *ctx) {
  graphics_draw_bitmap_in_rect(ctx, background_bmp, GRect(0,13,144,155));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_dots_ctx(GContext *ctx) {
  extern int8_t map[];
  graphics_context_set_stroke_color(ctx, GColorWhite);
  if(((animate>>2)&1)==0)
    graphics_context_set_fill_color(ctx, GColorBlack);
  else
    graphics_context_set_fill_color(ctx, GColorWhite);
  //graphics_context_set_fill_color(ctx, (animate>>2)&1);
  for(uint16_t y=0, i=0; y<MAP_H; y++)
    for(uint16_t x=0; x<MAP_W; x++, i++)
      if(map[i]&16) {       // big dot
        graphics_fill_rect(ctx, GRect(x*5+BOARD_X+1,y*5+BOARD_Y+1,3,3), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(x*5+BOARD_X+2,y*5+BOARD_Y,1,5), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(x*5+BOARD_X,y*5+BOARD_Y+2,5,1), 0, GCornerNone);
        //graphics_draw_pixel(ctx, GPoint(x*5+BOARD_X+2, y*5+BOARD_Y+2));
      } else if(map[i]&8) { // regular dot
        graphics_draw_pixel(ctx, GPoint(x*5+BOARD_X+2, y*5+BOARD_Y+2));
      }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_top_ctx(GContext *ctx) {   // Top means the top 13 pixels, including score, lives, current player, level and bonus
  extern PlayerStruct player[];
  extern uint8_t current_player;

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, 144, 13), 0, GCornerNone);
  
  static char text[40];  //Buffer to hold text
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  //snprintf(text, sizeof(text), "%ld", (long int)QQ);
  //snprintf(text, sizeof(text), "Score:%ld Lives:%ld", (long int)player[current_player].score, (long int)player[current_player].lives);
//   snprintf(text, sizeof(text), "%d %d %ld %ld", accel.x, accel.y, atan2_lookup(accel.z, accel.x), atan2_lookup(-1 * accel.y, accel.z));
  
  //snprintf(text, sizeof(text), "(%ld, %ld) %d", player[currentplayer].pos.x, player[currentplayer].pos.y, getmap(player[currentplayer].pos.x, player[currentplayer].pos.y));
  //snprintf(text, sizeof(text), "123456789012345678901234");  // Max for "RESOURCE_ID_GOTHIC_14" and "RESOURCE_ID_GOTHIC_14_BOLD"
  graphics_draw_text(ctx, text, fonts_get_system_font("RESOURCE_ID_GOTHIC_14_BOLD"), GRect(0, -4, 144, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  // draw score (6 digits max -- 7 digits technically possible on level 100)
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //












#ifdef PBL_BW
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
//  Black and White Drawing Functions
// ------------------------------------------------------------------------ //
void draw_background_fb(uint8_t *fb) {
 for (uint16_t i=0; i<BOARD_Y*5; i++) ((uint32_t*)fb)[i] = 0; // Top rows black
 for (uint16_t i=0, j=BOARD_Y*5; i<155*5; i++,j++) ((uint32_t*)fb)[j] = ((uint32_t*)background)[i]; // Draw Background Image -- equivilant to: graphics_draw_bitmap_in_rect(ctx, background, GRect(0,BOARD_Y,144,155));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //

#define   pset(x, y) fb[((y)*20) + ((x) >> 3)] |=  (1 << ((x)&7))
#define pclear(x, y) fb[((y)*20) + ((x) >> 3)] &= ~(1 << ((x)&7))
//uint8_t xaddr = (x*5+BOARD_X+2) >> 3;
//uint8_t xbit  = (x*5+BOARD_X+2) & 7;
//fb[((y*5+BOARD_Y+2)*20) + xaddr] |= 1 << xbit;

void draw_dots_fb(uint8_t *fb) {
  extern int8_t map[];
  for(uint16_t y=0, i=0; y<MAP_H; y++)
    for(uint16_t x=0; x<MAP_W; x++, i++)
      if(map[i]&16) {      // big dot
        if(((animate>>2)&1) == 1) {
          uint16_t px=x*5+BOARD_X, py=y*5+BOARD_Y;
          pset(px+2, py+0);
          pset(px+1, py+1);
          pset(px+2, py+1);
          pset(px+3, py+1);
          pset(px+0, py+2);
          pset(px+1, py+2);
          pset(px+2, py+2);
          pset(px+3, py+2);
          pset(px+4, py+2);
          pset(px+1, py+3);
          pset(px+2, py+3);
          pset(px+3, py+3);
          pset(px+2, py+4);
        }
      } else if(map[i]&8) { // regular dot
        pset(x*5+BOARD_X+2, y*5+BOARD_Y+2);
      }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_pupils(uint8_t *fb, int32_t x, int32_t y, uint8_t color, uint8_t facing) {
  x = ((x>>6)*ZOOM) + (((x&63)*ZOOM)/64) + BOARD_X;
  y = ((y>>6)*ZOOM) + (((y&63)*ZOOM)/64) + BOARD_Y;
  switch(facing) {
    case 0:  //00
      pclear(x, y);
      pclear(x+3, y);
    break;
    case 1:  //01
      pclear(x-1, y-2);
      pclear(x+1, y-2);
    break;
    case 2:  //10
      pclear(x, y);
      pclear(x-3, y);
    break;
    case 3:  //11
      pclear(x-1, y+1);
      pclear(x+1, y+1);
    break;
  }
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
      for(addr.x++; addr.x<(rect.size.w>>3); addr.x++)        if(addr.x >= 0 && addr.x < 19) screen[addr.y + addr.x] = data[y&1]; // fill middle of row
      if  (addr.x >= 0 && addr.x < 19) screen[addr.y + addr.x] = (screen[addr.y + addr.x] & r_mask) + (data[y&1] & ~r_mask); // fill right-side of row
    }
  }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
#endif

  
  
  
  
  
  
  
  
#ifdef PBL_COLOR
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
//  Color Drawing Functions
// ------------------------------------------------------------------------ //
//void draw_background_fb(uint8_t *fb) {
//  for (uint16_t i=0; i<BOARD_Y*5; i++) ((uint32_t*)(fb->addr))[i] = 0; // Top 13 rows of black -- equivilant to: graphics_context_set_fill_color(ctx, 0); graphics_fill_rect(ctx, GRect(0,0,144,12), 0, GCornerNone);
//  for (uint16_t i=0,j=BOARD_Y*5; i<155*5; i++,j++) ((uint32_t*)(fb->addr))[j] = ((uint32_t*)background->addr)[i]; // Draw Background Image -- equivilant to: graphics_draw_bitmap_in_rect(ctx, background, GRect(0,13,144,155));
//}
void draw_background_fb(uint8_t *fb) {
  for(uint16_t i=0; i<BOARD_Y*144; ++i) ((uint32_t*)fb)[i] = 0b11000000; // Top rows black
  
  uint32_t fbpos=BOARD_Y*144;
  for(uint32_t j=0; j<155*144; ++j)
    fb[fbpos++] = ((background[j>>3] >> (7-(j&7))) & 1) ? 0b11000011 : 0b11000000;  // draw board blue with black background

  //Spectre Door
  for(fbpos = (BOARD_Y+63)*144 + 67; fbpos<(BOARD_Y+63)*144 + 67 + 10; ++fbpos)
    fb[fbpos] = 0b11111010;

//    ((uint32_t*)fb)[j] = background[i]; // Draw Background Image -- equivilant to: graphics_draw_bitmap_in_rect(ctx, background, GRect(0,13,144,155));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_dots_fb(uint8_t *fb) {
  #define dotcolor 0b11111111
  extern int8_t map[];
  uint32_t addr = ((BOARD_Y+2)*144)+BOARD_X+2;
  for(uint16_t y=0, i=0; y<MAP_H; ++y, addr+=(144*4) + (144-(5*MAP_W)))
    for(uint16_t x=0; x<MAP_W; ++x, ++i, addr+=5)
      if(map[i]&16) {        // big dot
        if((animate>>2)&1) {
          fb[addr-144-144] = dotcolor;
          fb[addr-144-1] = dotcolor;
          fb[addr-144] = dotcolor;
          fb[addr-144+1] = dotcolor;
          fb[addr-2] = dotcolor;
          fb[addr-1] = dotcolor;
          fb[addr] = dotcolor;
          fb[addr+1] = dotcolor;
          fb[addr+2] = dotcolor;
          fb[addr+144-1] = dotcolor;
          fb[addr+144] = dotcolor;
          fb[addr+144+1] = dotcolor;
          fb[addr+144+144] = dotcolor;
        }
      } else if(map[i]&8) {  // regular dot
        fb[addr] = dotcolor;
      }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
void draw_pupils(uint8_t *fb, int32_t x, int32_t y, uint8_t color, uint8_t facing) {
  //NOTE: Will probably break when going out of screen bounds
  x=(((x>>6)*ZOOM) + (((x&63)*ZOOM)/64) + BOARD_X);
  y=(((y>>6)*ZOOM) + (((y&63)*ZOOM)/64) + BOARD_Y);
  uint32_t addr = (y*144)+x;
  if(facing&1) {
    addr += ((facing&2)?+144:-288);
    fb[addr - 1]=0;
    fb[addr + 1]=0;
  } else {
      fb[addr]=0;
      fb[addr + ((facing&2)?-3:3)]=0;
  }
//   switch(facing) {
//     case 0:  //00
//       fb[addr]=0;
//       fb[addr+3]=0;
//     break;
//     case 1:  //01
//       fb[addr-144-144-1]=0;
//       fb[addr-144-144+1]=0;
//     break;
//     case 2:  //10
//       fb[addr]=0;
//       fb[addr-3]=0;
//     break;
//     case 3:  //11
//       fb[addr+144-1]=0;
//       fb[addr+144+1]=0;
//     break;
//   }
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
  
  
  
  
  
  
  
  
  
  
  
  
  
// void draw_sprite8(uint8_t *fb, int16_t start_x, int16_t start_y, uint8_t hflip, uint8_t vflip, uint8_t color, uint8_t spr) {  // uint8_t color isn't used in B&W
//   hflip = hflip ? 1 : 0;  vflip = vflip ? 1 : 0;  // makes sure hflip and vflip are either 1 or 0
//   uint16_t left   = (start_x <     0) ? (start_x >  -8) ?   0 - start_x : 8 : 0;
//   uint16_t right  = (start_x > 144-8) ? (start_x < 144) ? 144 - start_x : 0 : 8;
//   uint16_t top    = (start_y <     0) ? (start_y >  -8) ?   0 - start_y : 8 : 0;
//   uint16_t bottom = (start_y > 168-8) ? (start_y < 168) ? 168 - start_y : 0 : 8;
  
//   uint8_t    *row = font8+(spr&3) + ((spr&252)*8) + ((vflip?bottom:top)*4);
//   uint16_t y_addr = (start_y + top) * IF_COLORBW(144, 20);
  
//   for(uint16_t y=top; y<bottom; ++y) {
//     for(uint16_t x=left; x<right; ++x) {
//       #ifdef PBL_BW
//         //fb[y_addr + ((start_x+x) >> 3)] &= ~(1 << ((start_x+x)&7)); // Black Background (comment both out for clear background)
//         //fb[y_addr + ((start_x+x) >> 3)] |=  (1 << ((start_x+x)&7)); // White Background (comment both out for clear background)
//         //fb[y_addr + ((start_x+x) >> 3)] &= ~((((*row>>x)&1)) << ((start_x+x)&7)); // Black Pixel
//           fb[y_addr + ((start_x+x) >> 3)] |=  ((((*row>>x)&1)) << ((start_x+x)&7)); // White Pixel
//       #else
//         //if((*row>>x)&1)   // horizontally flipped
//         if((*row&(128>>x))) // normal
//           fb[y_addr + start_x + x] = color;
//           // else fb[y_addr + x] = background_color;
//       #endif
//     }
//     y_addr += IF_COLORBW(144, 20);
//     row += vflip ? -4 : 4;
//   }
// }


void draw_sprite8(uint8_t *fb, uint8_t *font, int16_t start_x, int16_t start_y, uint8_t color, uint8_t spr) {  // in B&W, color=0 is black, else white
  uint16_t left   = (start_x <     0) ? (start_x >  -8) ?   0 - start_x : 8 : 0;
  uint16_t right  = (start_x > 144-8) ? (start_x < 144) ? 144 - start_x : 0 : 8;
  uint16_t top    = (start_y <     0) ? (start_y >  -8) ?   0 - start_y : 8 : 0;
  uint16_t bottom = (start_y > 168-8) ? (start_y < 168) ? 168 - start_y : 0 : 8;
  uint8_t    *row = font + (spr&3) + ((spr&252)*8) + (top*4);
  uint16_t y_addr = (start_y + top) * IF_COLORBW(144, 20);
  
  for(uint16_t y=top; y<bottom; ++y) {
    for(uint16_t x=left; x<right; ++x) {
      #ifdef PBL_BW
        //fb[y_addr + ((start_x+x) >> 3)] &= ~(1 << ((start_x+x)&7)); // Black Background (comment both out for clear background)
        //fb[y_addr + ((start_x+x) >> 3)] |=  (1 << ((start_x+x)&7)); // White Background (comment both out for clear background)
        if(color)
          fb[y_addr + ((start_x+x) >> 3)] |=  ((((*row>>x)&1)) << ((start_x+x)&7)); // White Pixel
        else
          fb[y_addr + ((start_x+x) >> 3)] &= ~((((*row>>x)&1)) << ((start_x+x)&7)); // Black Pixel
      #else
        //if((*row>>x)&1)   // horizontally flipped
        if((*row&(128>>x))) // normal
          fb[y_addr + start_x + x] = color;
          // else fb[y_addr + x] = background_color;
      #endif
    }
    y_addr += IF_COLORBW(144, 20);
    row += 4;
  }
}

void draw_font8_text(uint8_t *fb, int16_t x, int16_t y, uint8_t color, char *str) { // str points to zero-terminated string
  uint8_t strpos=0;
  while(str[strpos]>0) {
    if(x>136) {x=0; y+=8;}  // wrap horizontally
    if(y>160) y=0;          // wrap vertically
    draw_sprite8(fb, font8, x, y, color, str[strpos]);
    x+=8; strpos++;
  }
}

void draw_actor(uint8_t *fb, int32_t x, int32_t y, uint8_t color, uint8_t spr) {
    draw_sprite8(fb, font_sprites, (((x>>6)*5) + (((x&63)*5)/64) + BOARD_X - 3), (((y>>6)*5) + (((y&63)*5)/64) + BOARD_Y - 3), color, spr);
  
  
  IF_COLOR(fb[((((y>>6)*5) + (((y&63)*5)/64) + BOARD_Y)*144)+(((x>>6)*5) + (((x&63)*5)/64) + BOARD_X)]=0b11001100);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //
//  Pattern Functions
// ------------------------------------------------------------------------ //
Layer *pattern_layer;
static uint8_t *pattern;
// uint8_t brickpattern[] = { 21, 10, 68, 160, 81,162, 69,138, 0b11110000, 0b11000000};  //8 bytes, white, black
uint8_t brickpattern[] = { 0, 0, 0, 0, 0,0, 0,0, 0b11110000, 0b11000000};  //8 bytes, white, black

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
///////  layer_add_child(root_layer, pattern_layer);
  pattern=brickpattern;
//   patterntimer = app_timer_register(50, move_pattern, NULL);
}

void destroy_pattern_layer() {
  layer_destroy(pattern_layer);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------ //
