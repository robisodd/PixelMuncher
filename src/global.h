#pragma once
#include "pebble.h"
#define UPDATE_MS 30 // Refresh rate in milliseconds (about 32fps)
#define ZOOM 5       // Number of pixels per map square
// Map SHOULD be:
// Currently: 28w x 31h
// Original:  28w x 36h
// Should be: 32w x 36h
#define MAP_W 28
#define MAP_H 31
  
// Pixel Offset to display board on screen
#define BOARD_X 2
#define BOARD_Y 13
  
//Cardinal Directions = facing >> 15... maybe.
#define East  0 // +x
#define South 1 // +y
#define West  2 // -x
#define North 3 // -y

// =========================================================================================================== //
typedef struct XYStruct {
  int32_t x;
  int32_t y;
} XYStruct;
// =========================================================================================================== //
#define Cherries   0
#define Strawberry 1
#define Peach      2
#define Apple      3
#define Grapes     4
#define Galaxian   5
#define Bell       6
#define Key        7
typedef struct BonusStruct {
  GBitmap *sprite;
  uint16_t points;
} BonusStruct;
//BonusStruct bonus[8] = (BonusStruct){{NULL,100},};
//Ya know what?  No.  Gonna load the bonus sprite at the beginning of each level.
  

// ============================
//            Bonus
// ============================
//  Index   Points     Name
//    0       100    Cherries  
//    1       300    Strawberry
//    2       500    Peach
//    3       700    Apple
//    4      1000    Grapes
//    5      2000    Galaxian
//    6      3000    Bell
//    7      5000    Key
// ============================

// =========================================================================================================== //
int32_t abs32(int32_t x);
