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
//#define BOARD_H 29 // technically 31 with top/bottom border, but can leave those out
  
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

  //PlayerStruct are things about the player
  //  which are the things that stay with "player 1" and "Player 2" etc
  // including initials, current dots remaining, score, lives, current level
  //
  // muncher properties reset between each player and/or single player death
  //   including: x,y pos, speed, mouth frame
  
typedef struct PlayerStruct {
  uint32_t score;
   uint8_t lives;
   uint8_t level;
   uint8_t dots[31];
  // name/initials?
} PlayerStruct;

void init_player(uint8_t ID);
void create_players(uint8_t num_of_players);
// void add_points(uint32_t points);

//uint8_t get_current_player();
// uint8_t get_lives();
// uint32_t get_score();
// uint8_t get_level();


//TODO: replace below with: typedef struct {
  
typedef struct MuncherStruct {
// 	uint16_t x, y; // in pixels, not tile -- center pixel
  XYStruct pos;
  XYStruct dir;    // direction of movement (always -1, 0 or 1)
   uint8_t speed;  // speed multlipier for direction of movement
  //uint8_t face;  // 0=Left, 1=Up, 2=Right, 3=Down
   int16_t facing; // Eater Direction Facing (from 0 - TRIG_MAX_ANGLE)
  uint32_t frame;  // Animation frame. Mouth: 0=Closed, 1=Open, 2=Wide, 3=Open
} MuncherStruct;


void init_muncher();
void move_muncher();
void muncher_eat_dots();

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


#define AccelerometerControl   0 // Accelerometer Movement (default)
#define ULDRButtonControl      1 // Up/Down/Back/Select for Up/Down/Left/Right
#define LRButtonControl        2 // Up/Down for CounterClockwise/Clockwise rotation, Select to reverse direction

#define AccelerometerTolerance 10 // How far to tilt watch before it's recognized as joystick input
  
void update_movement_via_joystick();
void game_click_config_provider(void *context);



extern AccelData accel;
extern uint16_t totalpellets;
extern uint8_t dotflashing;
extern uint8_t speed;         // probably replace with level[currentlevel].playerspeed

void load_graphics();
void draw_background_ctx(GContext *ctx);
void draw_dots_ctx(GContext *ctx);
void draw_muncher_ctx(GContext *ctx);
void draw_top_ctx(GContext *ctx);

void build_shadow_table();
void fill_rect(uint8_t *screen, GRect rect, uint8_t color);

void set_pattern(uint8_t *data);
void fill_framebuffer_with_pattern(uint8_t *screen, uint8_t *data);
void modify_pattern(uint8_t *data, int8_t x_offset, int8_t y_offset, uint8_t invert);
void create_pattern_layer();
void destroy_pattern_layer();


typedef struct Layer
{
  GRect bounds;
  GRect frame;
  bool clips : 1;
  bool hidden : 1;
  struct Layer *next_sibling;
  struct Layer *parent;
  struct Layer *first_child;
  struct Window *window;
  LayerUpdateProc update_proc;
} Layer;

void mainmenu();

void intro();
void draw_font8(uint8_t *screen, int16_t x, int16_t y, uint8_t color, uint8_t chr);