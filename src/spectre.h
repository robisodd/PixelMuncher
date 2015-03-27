#pragma once
#include "global.h"
// =========================================================================================================== //
typedef struct SpectreStruct {
// 	uint16_t x, y; // in pixels, not tile -- center pixel
  XYStruct pos;
  XYStruct dir;   // direction of movement (always -1, 0 or 1)
   uint8_t speed; // speed multlipier for direction of movement
  //uint8_t face;  // 0=Left, 1=Up, 2=Right, 3=Down
   int16_t facing;             // Eater Direction Facing (from 0 - TRIG_MAX_ANGLE)
  uint32_t frame; // Animation frame. Mouth: 0=Closed, 1=Open, 2=Wide, 3=Open
  
} SpectreStruct;
SpectreStruct spectre[4];
// =========================================================================================================== //
