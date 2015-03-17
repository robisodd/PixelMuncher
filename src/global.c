#include <pebble.h>
#include "global.h"
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
