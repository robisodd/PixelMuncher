#include <pebble.h>
#include "input.h"
#include "muncher.h"
#include "map.h"

const int32_t face_to_dir[2][4] = {{1, 0, -1, 0},{0, -1, 0, 1}}; // X then Y
  
// =========================================================================================================== //
//  Control Input Options
// ======================= //
bool up_button_depressed = false; // Whether Pebble's   Up   button is being held
bool dn_button_depressed = false; // Whether Pebble's  Down  button is being held
bool sl_button_depressed = false; // Whether Pebble's Select button is being held
bool bk_button_depressed = false; // Whether Pebble's  Back  button is being held

//uint16_t currently_at;
uint16_t   turning_at = 32*32;  // Stops turning  left every frame when  UP  is held
//uint16_t     right_at = 32*32;  // Stops turning right every frame when DOWN is held

//uint8_t control_mode = AccelerometerControl;
//uint8_t control_mode = ULDRButtonControl;
uint8_t control_mode = LRButtonControl;


// ------------------------------------------------------------------------ //
//  Button Pushing
// ------------------------------------------------------------------------ //
void up_push_in_handler(ClickRecognizerRef recognizer, void *context) {up_button_depressed = true;  }  //   UP   button was pushed in
void up_release_handler(ClickRecognizerRef recognizer, void *context) {up_button_depressed = false; }  //   UP   button was released
void dn_push_in_handler(ClickRecognizerRef recognizer, void *context) {dn_button_depressed = true;  }  //  DOWN  button was pushed in
void dn_release_handler(ClickRecognizerRef recognizer, void *context) {dn_button_depressed = false; }  //  DOWN  button was released
void sl_push_in_handler(ClickRecognizerRef recognizer, void *context) {sl_button_depressed = true;  }  // SELECT button was pushed in
void sl_release_handler(ClickRecognizerRef recognizer, void *context) {sl_button_depressed = false; }  // SELECT button was released
void bk_click_handler  (ClickRecognizerRef recognizer, void *context) {bk_button_depressed = false; }  //  BACK  button was clicked (BACK doesn't support raw)
// BACK  button was pushed in
// seems BACK might do this

void game_click_config_provider(void *context) {
  window_raw_click_subscribe(BUTTON_ID_UP, up_push_in_handler, up_release_handler, context);
  window_raw_click_subscribe(BUTTON_ID_DOWN, dn_push_in_handler, dn_release_handler, context);
  window_raw_click_subscribe(BUTTON_ID_SELECT, sl_push_in_handler, sl_release_handler, context);
  //window_raw_click_subscribe(BUTTON_ID_BACK, bk_push_in_handler, bk_release_handler, context);
  window_single_click_subscribe(BUTTON_ID_BACK, bk_click_handler);
}


// ------------------------------------------------------------------------ //
//  Get Joystick, Update Eater
// ------------------------------------------------------------------------ //
/* TODO:
         Use ARCTAN2 to figure X&Y Joystick by XZ and YZ accelerometer
         Maybe subtle movement = centered joystick position
*/
void update_movement_via_joystick() {
  AccelData accel;
  XYStruct testspeed, testfacing;
  extern MuncherStruct muncher;
  
  testspeed.x = 0; testspeed.y = 0;  testfacing.x = 0;  testfacing.y = 0;
  switch(control_mode) {
    case AccelerometerControl:
      accel_service_peek(&accel); // Read accelerometer
      accel.x>>=3; accel.y>>=3;
           if(accel.x<-AccelerometerTolerance) {if(getmap(muncher.pos.x+(64*-1), muncher.pos.y+(64* 0))>=0) {testspeed.x =-1; testfacing.x = 2;}} // Left
      else if(accel.x> AccelerometerTolerance) {if(getmap(muncher.pos.x+(64* 1), muncher.pos.y+(64* 0))>=0) {testspeed.x = 1; testfacing.x = 0;}} // Right
           if(accel.y<-AccelerometerTolerance) {if(getmap(muncher.pos.x+(64* 0), muncher.pos.y+(64* 1))>=0) {testspeed.y = 1; testfacing.y = 3;}} // Down
      else if(accel.y> AccelerometerTolerance) {if(getmap(muncher.pos.x+(64* 0), muncher.pos.y+(64*-1))>=0) {testspeed.y =-1; testfacing.y = 1;}} // Up
    
      if((abs32(accel.x)>abs32(accel.y) || testspeed.y==0) && testspeed.x!=0) {
        muncher.dir.x = testspeed.x;
        muncher.dir.y = 0;
        muncher.facing  = testfacing.x;
      } else if(testspeed.y!=0) {
        muncher.dir.x = 0;
        muncher.dir.y = testspeed.y;
        muncher.facing  = testfacing.y;
      }
      break;
    
    //TODO: Unfinished ULDR - doesn't test if wall is there
    case ULDRButtonControl:
      //if(bk_button_depressed && !sl_button_depressed) {if(getmap(muncher.pos.x+(64*-1), muncher.pos.y+(64* 0))>=0) {testspeed.x =-1; testfacing.x = 2;}} // Left
      //if(sl_button_depressed && !bk_button_depressed) {if(getmap(muncher.pos.x+(64* 1), muncher.pos.y+(64* 0))>=0) {testspeed.x = 1; testfacing.x = 0;}} // Right
      //if(dn_button_depressed && !up_button_depressed) {if(getmap(muncher.pos.x+(64* 0), muncher.pos.y+(64* 1))>=0) {testspeed.y = 1; testfacing.y = 3;}} // Down
      //if(up_button_depressed && !dn_button_depressed) {if(getmap(muncher.pos.x+(64* 0), muncher.pos.y+(64*-1))>=0) {testspeed.y =-1; testfacing.y = 1;}} // Up
      if(bk_button_depressed && !sl_button_depressed) {if(getmap(muncher.pos.x+(64*-1), muncher.pos.y+(64* 0))>=0) {muncher.dir.y=0;muncher.dir.x=-1; muncher.facing=2;}} // Left
      if(sl_button_depressed && !bk_button_depressed) {if(getmap(muncher.pos.x+(64* 1), muncher.pos.y+(64* 0))>=0) {muncher.dir.y=0;muncher.dir.x= 1; muncher.facing=0;}} // Right
      if(dn_button_depressed && !up_button_depressed) {if(getmap(muncher.pos.x+(64* 0), muncher.pos.y+(64* 1))>=0) {muncher.dir.x=0;muncher.dir.y= 1; muncher.facing=3;}} // Down
      if(up_button_depressed && !dn_button_depressed) {if(getmap(muncher.pos.x+(64* 0), muncher.pos.y+(64*-1))>=0) {muncher.dir.x=0;muncher.dir.y=-1; muncher.facing=1;}} // Up    
    break;
    
    case LRButtonControl:         // Up/Down = CounterClockwise/Clockwise, Select = Reverse Direction (back brings up menu)
      //if pushing SL
      //  reverse
      //else
      //  if can go ccw and pushing UP then
      //    if can go cw and pushing DN then
      //      don't move
      //    else
      //      go ccw(up)
      //  else
      //    if can go cw(down) and pushing down
      //      go cw(down)
  
    
      if(sl_button_depressed) {            // Reverse
         sl_button_depressed = false;       // Stop reversing
         muncher.facing = (muncher.facing+2)&3;
         muncher.dir.x = face_to_dir[0][muncher.facing];
         muncher.dir.y = face_to_dir[1][muncher.facing];
       } else if(((muncher.pos.x>>6) * (muncher.pos.y>>6)) != turning_at) {
         turning_at = 32*32;
         testfacing.x = (muncher.facing+1)&3; // Left Turn
         testfacing.y = (muncher.facing+3)&3; // Right Turn
         
         if(getmap(muncher.pos.x+(64*face_to_dir[0][testfacing.x]), muncher.pos.y+(64*face_to_dir[1][testfacing.x]))>=0 && up_button_depressed) {
           if(getmap(muncher.pos.x+(64*face_to_dir[0][testfacing.y]), muncher.pos.y+(64*face_to_dir[1][testfacing.y]))>=0 && dn_button_depressed) {
           } else {
             //go ccw(up)
             muncher.facing = testfacing.x;
             muncher.dir.x = face_to_dir[0][muncher.facing];
             muncher.dir.y = face_to_dir[1][muncher.facing];
             turning_at = ((muncher.pos.x>>6) * (muncher.pos.y>>6));
           }
         } else {
           if(getmap(muncher.pos.x+(64*face_to_dir[0][testfacing.y]), muncher.pos.y+(64*face_to_dir[1][testfacing.y]))>=0 && dn_button_depressed) {
             //go cw(down)
             muncher.facing = testfacing.y;
             muncher.dir.x = face_to_dir[0][muncher.facing];
             muncher.dir.y = face_to_dir[1][muncher.facing];
             turning_at = ((muncher.pos.x>>6) * (muncher.pos.y>>6));
           }
         }
     }
    break;
  }
}
//   testspeed.x = 0; testspeed.y = 0;
//   if(joystickmode) {
//     if(abs32(accel.x)>abs32(accel.y)) {
//       if(accel.x<0) {testspeed.x =-1; testfacing = 2;} // Left
//       else          {testspeed.x = 1; testfacing = 0;} // Right
//     } else {
//       if(accel.y<0) {testspeed.y = 1; testfacing = 3;} // Down
//       else          {testspeed.y =-1; testfacing = 1;} // Up
//     }
//   } else { // button mode
//     if(sl_button_depressed && player[currentplayer].facing != 0) {testspeed.x = 1; testfacing = 0;} // Right
//     if(up_button_depressed && player[currentplayer].facing != 1) {testspeed.y =-1; testfacing = 1;} // Up
//     if(bk_button_depressed && player[currentplayer].facing != 2) {testspeed.x =-1; testfacing = 2;} // Left
//     if(dn_button_depressed && player[currentplayer].facing != 3) {testspeed.y = 1; testfacing = 3;} // Down
//   }
  
//   if(getmap(pos.x+(64*testspeed.x), pos.y+(64*testspeed.y))>=0) {  // if trying to turn and you can turn
//     player[currentplayer].speed.x=testspeed.x;
//     player[currentplayer].speed.y=testspeed.y;
//     player[currentplayer].facing = testfacing;
//   }
  