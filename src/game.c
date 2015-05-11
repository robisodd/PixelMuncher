//56b37c6f-792a-480f-b962-9a0db8c32aa4
//b00bface-effd-480f-b962-c0ded4c0ffee
#include "global.h"
// How to draw font
//   GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
//   if(framebuffer) {   // if successfully captured the framebuffer
//     uint8_t* screen = gbitmap_get_data(framebuffer);
//     draw_font8(screen, 5, 5, 1, 65);
//     graphics_release_frame_buffer(ctx, framebuffer);
//   }  // endif successfully captured framebuffer
  
//uint16_t totalpellets;  // delete this, not needed anymore

// ------------------------------------------------------------------------ //
//  Helper Functions
// ------------------------------------------------------------------------ //
int32_t abs32(int32_t x) {return (x ^ (x >> 31)) - (x >> 31);}

extern Window *main_window;
extern Layer *root_layer;
Layer *game_layer;
AppTimer* looptimer;

uint8_t dotflashing=0;

PlayerStruct player[5];
uint8_t currentplayer = 0;
uint8_t current_player;
uint8_t max_players;

uint8_t get_next_player() {
  //for players current+1 mod max to max: if lives>0 return it
  //for players = 1 to current, if lives>0 return it
  //all players have 0 lives, if so, return 255
  //set_current_player()
 return 0;
}

void init_player(uint8_t ID) {
  player[ID].score = 0;
  player[ID].level = 1;
  player[ID].lives = 3;
  for(uint8_t i=0; i<31; i++)
    player[ID].dots[i] = 0; // all dots = uneaten
  //name="Player " & Number
}

void create_players(uint8_t num_of_players) {
  if(num_of_players>4) num_of_players=4;  // Stick with single digits (1-9, no player 0)
  // if exists, destroy player[]
  // malloc player[]
  max_players = num_of_players;
  for(uint8_t i=0; i<max_players; i++) {
    init_player(i);
  }
  current_player = 0;
}

void destroy_players() {
  //free malloc player[]
}


// void add_points(uint32_t points) {player[current_player].score+=points;}
// uint32_t get_score() {return player[current_player].score;}
//  uint8_t get_lives() {return player[current_player].lives;}
//  uint8_t get_level() {return player[current_player].level;}
//  uint8_t get_current_player() {return current_player;}
//bool player_is_dead(
//void set_current_player(uint8_t ID) {current_player = ID;}

static void init_spectres() {
  //Enemy Init:
	//Start Positions
	//Target Coordinates for every mode (attack, regroup, scare)

  //Modes:
  // Bunker
  // Patrol
  // Attack
  // Crusie Elroy
  // Scare (Blue)
  // Dead/Eyes
  
}

MuncherStruct muncher;

void init_muncher() {
  muncher.pos.x   = (14<<6);    // start halfway between 13&14;
  muncher.pos.y   = (23<<6)+32; // start in middle of 23;
  muncher.facing  = 1;          // Facing Up. if using angles, then TRIG_MAX_ANGLE/4, or 1<<13; (up so counterclockwise button goes left, clockwise goes right)
  muncher.frame   = 0;          // Full Circle Sprite
  muncher.dir.x = 0;
  muncher.dir.y = 0;
}

void move_muncher() {
  //TODO: tend toward the middle based on speed
  if(muncher.dir.x != 0) {     // if moving horizontally
    if(getmap(muncher.pos.x+(32*muncher.dir.x), muncher.pos.y)>=0) { // if not running into a wall  
      muncher.pos.x += (muncher.speed*muncher.dir.x);
      if(muncher.pos.x<-63)     muncher.pos.x += (32<<6);     // tunnel left wraparound
      if(muncher.pos.x>(32<<6)) muncher.pos.x -= (32<<6); // tunnel right wraparound
      muncher.frame=(muncher.frame+1)&3;
      muncher.pos.y = ((muncher.pos.y>>6)<<6)+32; // tend toward tile center
    } else { // will hit a wall
      muncher.pos.x = ((muncher.pos.x>>6)<<6)+32; // finish moving toward wall, stop at tile center
      muncher.dir.x = 0;
    }
  } else if(muncher.dir.y !=0) {  // (not moving horizontally) if moving vertically
    if(getmap(muncher.pos.x, muncher.pos.y+(32*muncher.dir.y))>=0) { // if not running into a wall  
      muncher.pos.y += (muncher.speed*muncher.dir.y);
      muncher.frame=(muncher.frame+1)&3;
      muncher.pos.x = ((muncher.pos.x>>6)<<6)+32; // tend toward tile center
    } else { // hit a wall
      muncher.pos.y = ((muncher.pos.y>>6)<<6)+32; // finish moving toward wall, stop at tile center
      muncher.dir.y = 0;
    }
  } else { } // (not moving horizontally, not moving vertically)
}

void muncher_eat_dots() {
  //====================================//
  // Eat Dots, Update Score, Slow Speed
  //====================================//
  if(getmap(muncher.pos.x, muncher.pos.y)==1) {  // if on a regular dot, eat dot
    muncher.speed -= 2;                                                   // slow down due to eating dot
//     add_points(10);
    player[current_player].score += 10;                                   // add 10 points to score
    setmap(muncher.pos.x, muncher.pos.y, 0); // remove dot from board
  }
  if(getmap(muncher.pos.x, muncher.pos.y)==2) {  // if on a super dot, eat superdot
    muncher.speed -= 6;                                                   // slow down due to eating superdot
//     add_points(50);
    player[current_player].score += 50;                                   // add 50 points to score
    setmap(muncher.pos.x, muncher.pos.y, 0); // remove superdot from board
    // [TODO] start CHASE mode
  }

}


// =========================================================================================================== //
//  Map and Level Variables
// ======================= //
//TODO:
// Init Map and Init Dots
// initial_dots will be just 36(?) 32bit ints, each bit will be 1 or 0 and the two 2s (superdots) will be done in function
// initial_map will also be just 1bit, converted to -1 or 0 in MAP[]
// player[].dots[] will be like initial_dots and use same function to copy to MAP[]

static int8_t boardlayout[] =
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

//00=impass 01=blank 10=dot 11=bigdot
uint32_t BoardLayout[31] = {
//00000000000000000000000000000000
//00000000000000000000000000000000
//00000000000000000000000000000000
//00000000000000000000000000000000 <- top of board
0b0000000000000000000000000000,
0b0010101010101010101010101000,
0b0010000000000010000000001000,
0b0010000101010010000101001100,
0b0010000000000010000000001000,
0b1010101010101010101010101000,
0b0000000010000010000000001000,
0b0000000010000010000000001000,
0b0010101010000010101010101000,
0b0001000000000010000000000000,
0b0001000000000010000101010101,
0b0101010101000010000101010101,
0b0000000001000010000101010101,
0b0101010001000010000000000000,
0b0101010001010110010101010101,
0b0101010001000010000000000000,
0b0000000001000010000101010101,
0b0101010101000010000101010101,
0b0000000001000010000101010101,
0b0000000001000010000000000000,
0b0010101010101010101010101000,
0b0010000000000010000000001000,
0b0010000000000010000000001000,
0b0110101010101010000010101100,
0b0000000010000010000010000000,
0b0000000010000010000010000000,
0b0010101010000010101010101000,
0b0010000000000000000000001000,
0b0010000000000000000000001000,
0b1010101010101010101010101000,
0b0000000000000000000000000000
//00000000000000000000000000000000 <- bottom of board
//00000000000000000000000000000000
//00000000000000000000000000000000
};



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
uint8_t levelplayerspeed;         // probably replace with level[min(player.level,21)].playerspeed

int8_t map[MAP_W * MAP_H];  // int8 means cells can be from -127 to 128

void save_dots(uint8_t *dots) {
  uint8_t bit = 0; // because 8 bits, can't have more than 256 total dots

  // convert -1,0,1,2 to 0,1,2,3
  for(uint16_t i=0; i<MAP_H*MAP_W; i++)
    map[i]++;

  for(uint16_t y=0, row=0; y<MAP_H*MAP_W; y+=MAP_W, row++)
    for(uint16_t x=0; x<(MAP_W/2); x++) {  // map_w has to <= 32, else bitshift below breaks
      if(((BoardLayout[row] >> (x*2))&3) > 1) { // if dot is supposed to exist
        if(map[y+x]>1) {
          //dots[(bit>>3)] &= 1(bit&7))&1
        }
        if(map[y + MAP_W - 1 - x]>1) {
          
        }
      }
      map[y+x]               = (BoardLayout[row] >> (x*2))&3;
      map[y + MAP_W - 1 - x] = (BoardLayout[row] >> (x*2))&3; // right 16 tiles = left side horizontally flipped
      if(map[y+x] > 1) {  // 2(0b10) = dot, 3(0b11) = bigdot
        if((dots[(bit>>3)]>>(bit&7))&1) // 1 = eaten
          map[y+x] = 1;                 // 1 = blank
        bit++;
        if((dots[(bit>>3)]>>(bit&7))&1) // 1 = eaten
          map[y + MAP_W - 1 - x] = 1;   // 1 = blank
        bit++;
      }
    }

}

void load_dots(uint8_t *dots) {
// note: in dots[], each bit is 0 if dot still exists or 1 if already eaten
// init player[].dots[] all to 0 to signify full board (0 = dot there in dots[])
  uint8_t bit = 0; // because 8 bits, can't have more than 256 total dots
  for(uint16_t y=0, row=0; y<MAP_H*MAP_W; y+=MAP_W, row++)
    for(uint16_t x=0; x<(MAP_W/2); x++) {  // map_w has to <= 32, else bitshift below breaks
      map[y+x]               = (BoardLayout[row] >> (x*2))&3;
      map[y + MAP_W - 1 - x] = (BoardLayout[row] >> (x*2))&3; // right 16 tiles = left side horizontally flipped
      if(map[y+x] > 1) {  // 2(0b10) = dot, 3(0b11) = bigdot
        if((dots[(bit>>3)]>>(bit&7))&1) // 1 = eaten
          map[y+x] = 1;                 // 1 = blank
        bit++;
        if((dots[(bit>>3)]>>(bit&7))&1) // 1 = eaten
          map[y + MAP_W - 1 - x] = 1;   // 1 = blank
        bit++;
      }
    }

  // convert 0,1,2,3 to -1,0,1,2
  for(uint16_t i=0; i<MAP_H*MAP_W; i++)
    map[i]--;
}

void init_board() {
//   for(uint16_t i=0; i<MAP_W*MAP_H; i++) map[i] = boardlayout[i];

  load_dots((uint8_t*)&player[current_player].dots);
  
/*  
  for(uint16_t y=0; y<MAP_H*MAP_W; y+=MAP_W)
   for(uint16_t x=0; x<(MAP_W/2); x++) {
     map[y+x] = boardlayout[(y/2)+x];
     map[y + MAP_W - 1 - x] = boardlayout[(y/2)+x];
   }
*/
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

//TODO: does face_do_dir need to be 32bits?
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
void bk_click_handler  (ClickRecognizerRef recognizer, void *context) {bk_button_depressed = true;  }  //  BACK  button was clicked (BACK doesn't support raw)

void game_click_config_provider(void *context) {
  window_raw_click_subscribe(BUTTON_ID_UP, up_push_in_handler, up_release_handler, context);
  window_raw_click_subscribe(BUTTON_ID_DOWN, dn_push_in_handler, dn_release_handler, context);
  window_raw_click_subscribe(BUTTON_ID_SELECT, sl_push_in_handler, sl_release_handler, context);
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
      if(bk_button_depressed) window_stack_pop_all(true); // back = quit (TODO: Menu)
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
    //TODO: Hold Up and Down together for a few frames = menu
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
  
      if(bk_button_depressed) window_stack_pop_all(true); // back = quit (TODO: Menu)
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
  bk_button_depressed = false; // since it's click, not raw, resetting bk variable until next depression
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
  

//Timer
//#define UPDATE_MS 50 // Refresh rate in milliseconds
//static AppTimer *timer;
//timer = app_timer_register(UPDATE_MS, timer_callback, NULL); // Schedule a callback, save handle in timer



//AccelData accel;// = (AccelData) { .x = 0, .y = 0, .z = 0 };
// ------------------------------------------------------------------------ //
//  Main Loop Functions
// ------------------------------------------------------------------------ //
static void gameloop(void *data) {
  extern MuncherStruct muncher;
  muncher.speed=getlevelspeed();

  muncher_eat_dots();
  update_movement_via_joystick();
  move_muncher();
  dotflashing++;
  
  layer_mark_dirty(game_layer);  // Schedule redraw of screen
  looptimer=NULL;
}




//====================================//
static void game_layer_update(Layer *me, GContext *ctx) {
//   GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
//   if(framebuffer) {
// //     draw_background(framebuffer);
//    graphics_release_frame_buffer(ctx, framebuffer);
//   }
  time_t sec1, sec2; uint16_t ms1, ms2, dt; // time snapshot variables, to calculate render time and FPS
  time_ms(&sec1, &ms1);  //1st Time Snapshot

  
  draw_background_ctx(ctx);
  draw_dots_ctx(ctx);
  draw_muncher_ctx(ctx);
  draw_top_ctx(ctx);

  time_ms(&sec2, &ms2);  //2nd Time Snapshot
  dt = (uint16_t)(1000*(sec2 - sec1)) + (ms2 - ms1);  //dt=delta time: time between two time snapshots in milliseconds
  player[current_player].score = dt;


  
//   app_timer_register(UPDATE_MS, gameloop, NULL); // Finished. Wait UPDATE_MS then loop
  if(!looptimer) looptimer = app_timer_register(UPDATE_MS, gameloop, NULL); // Finished. Wait UPDATE_MS then loop
}
  
// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
void init_game() {
  game_layer = root_layer;// create a new layer here //window_get_root_layer(window);
  
  layer_set_update_proc(game_layer, game_layer_update);
  
  load_graphics();
  init_board();
  init_muncher();
  create_players(0);
  currentplayer = 0;

  window_set_click_config_provider(main_window, game_click_config_provider);
}

void start_game() {
  looptimer = app_timer_register(UPDATE_MS, gameloop, NULL); // Finished. Wait UPDATE_MS then loop
//   app_timer_register(UPDATE_MS, gameloop, NULL); // Finished. Wait UPDATE_MS then loop
}

void destroy_game() {
  if(looptimer) app_timer_cancel(looptimer);
  accel_data_service_unsubscribe();
  //TODO: destroy game layer
  //destroy_graphics();
//  for (uint8_t i=0; i<4; i++) for (uint8_t j=0; j<4; j++) gbitmap_destroy(playersprite[i][j]);
//  gbitmap_destroy(background);
}