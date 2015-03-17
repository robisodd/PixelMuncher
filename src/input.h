#pragma once
#include "global.h"

#define AccelerometerControl   0 // Accelerometer Movement (default)
#define ULDRButtonControl      1 // Up/Down/Back/Select for Up/Down/Left/Right
#define LRButtonControl        2 // Up/Down for CounterClockwise/Clockwise rotation, Select to reverse direction

#define AccelerometerTolerance 10 // How far to tilt watch before it's recognized as joystick input
  
void update_movement_via_joystick();
void game_click_config_provider(void *context);