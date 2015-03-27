#include <pebble.h>
#include "map.h"

// =========================================================================================================== //
//  Map and Level Variables
// ======================= //
//TODO:
// Init Map and Init Dots
// initial_dots will be just 36(?) 32bit ints, each bit will be 1 or 0 and the two 2s (superdots) will be done in function
// initial_map will also be just 1bit, converted to -1 or 0 in MAP[]
// player[].dots[] will be like initial_dots and use same function to copy to MAP[]
static int8_t boardlayout[MAP_W * MAP_H / 2] =
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,
 -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,
 -1, 2,-1, 0, 0,-1, 1,-1, 0, 0, 0,-1, 1,-1,
 -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,
 -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 -1, 1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,
 -1, 1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,
 -1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1,-1,
 -1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 0,-1,
  0, 0, 0, 0, 0,-1, 1,-1,-1,-1,-1,-1, 0,-1,
  0, 0, 0, 0, 0,-1, 1,-1,-1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,-1, 1,-1,-1, 0,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1, 1,-1,-1, 0,-1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 0, 0, 0,-1, 0, 0, 0,
 -1,-1,-1,-1,-1,-1, 1,-1,-1, 0,-1, 0, 0, 0,
  0, 0, 0, 0, 0,-1, 1,-1,-1, 0,-1,-1,-1,-1,
  0, 0, 0, 0, 0,-1, 1,-1,-1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,-1, 1,-1,-1, 0,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1, 1,-1,-1, 0,-1,-1,-1,-1,
 -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,
 -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,
 -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,
 -1, 2, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 0,
 -1,-1,-1, 1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,
 -1,-1,-1, 1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,
 -1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1,-1,
 -1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,
 -1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,
 -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
//-1 = Impassable
// 0 = Blank
// 1 = Pellet
// 2 = Power Pellet

/*  Updated Map Data:
Map is now folded in half
bit 76543210
    xxxxxxxx = uint8_t
bit 0: 
bit 1: 
bit 2: 
bit 3: pellet
bit 4: power pellet
bit 5: spectre can't go up
bit 6: spectre goes slowly
bit 7: impassable (>127 means impassable)

Maybe bits 012 will be for 3D floor/ceiling squaretype

When bit 7 is on, the other bits change meaning:
bits 0-6: squaretype [0-127] (for 3D or 2D)

*/
LevelStruct level[21];
LevelStruct currentlevel;

int8_t map[MAP_W * MAP_H];  // int8 means cells can be from -127 to 128
uint8_t levelplayerspeed;         // probably replace with level[player.level].playerspeed

void init_board() {
//   for(uint16_t i=0; i<MAP_W*MAP_H; i++) map[i] = boardlayout[i];

 for(uint16_t y=0; y<MAP_H*MAP_W; y+=MAP_W)
  for(uint16_t x=0; x<(MAP_W/2); x++) {
    map[y+x] = boardlayout[(y/2)+x];
    map[y + MAP_W - 1 - x] = boardlayout[(y/2)+x];
  } 
  levelplayerspeed=19;  // Default 100% speed -- level speed will replace this
}
uint8_t getlevelspeed() {
  return levelplayerspeed;
}
int8_t getmap(int32_t x, int32_t y) {
  x>>=6; y>>=6;
  if(y==14 && (x<0 || x>=MAP_W)) return 0;
  return (x<0 || x>=MAP_W || y<0 || y>=MAP_H) ? -1 : map[(y * MAP_W) + x];
}

void setmap(int32_t x, int32_t y, int8_t data) {
  x>>=6; y>>=6;
  if(x>=0 && x<MAP_W && y>=0 && y<MAP_H)
    map[(y * MAP_W) + x]=data;
}
