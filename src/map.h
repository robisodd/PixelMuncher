#pragma once
#include "global.h"
typedef struct LevelStruct {
  uint16_t bonus;    //
  uint8_t eaterspeed;
  uint8_t spectrespeed;
  uint8_t bonussprite;
  uint32_t bonuspoints;
} LevelStruct;
// NOTE: Probably should change LevelStruct to a function which figures the data instead of a lookup table

void   init_board();
int8_t getmap(int32_t x, int32_t y);
void   setmap(int32_t x, int32_t y, int8_t data);
uint8_t getlevelspeed();