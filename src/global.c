#include <pebble.h>
#include "global.h"
  
Window *main_window;
Layer *root_layer;

uint8_t brickpattern[] = { 21, 10, 68, 160, 81,162, 69,138, 0b11110000, 0b11000000};  //8 bytes, white, black

  
/*
Definitions:

  Muncher: The circle character on the screen
  Player: The person controlling the Eater
  Spectre: Category term for bad guy
  
  Speedy: Red Spectre
  Name2
  Name3
  Name4
  Dot: single pixel Eater eats.  worth 10 points
  SuperDot: 4 Large dots, begins CHASEmode
  
  
  CHASEmode
  NORMALmode
  SCATTERmode
  othermodes






*/
  
// ------------------------------------------------------------------------ //
//  Helper Functions
// ------------------------------------------------------------------------ //
int32_t abs32(int32_t x) {return (x ^ (x >> 31)) - (x >> 31);}
