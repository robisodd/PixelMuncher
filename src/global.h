#pragma once
#include "pebble.h"

#ifdef PBL_COLOR
  #define IF_COLOR(statement) (statement)
  #define IF_BW(statement)
  #define IF_COLORBW(color, bw) (color)
#else
  #define IF_COLOR(statement)
  #define IF_BW(statement) (statement)
  #define IF_COLORBW(color, bw) (bw)
#endif
  
#define UPDATE_MS 30 // Refresh rate in milliseconds (about 32fps)
//#define UPDATE_MS 100 // Refresh rate in milliseconds (about 32fps)
  
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

// =========================================================================================================== //

#define Cherries   0
#define Strawberry 1
#define Peach      2
#define Apple      3
#define Grapes     4
#define Galaxian   5
#define Bell       6
#define Key        7
//uint32_t bonuspoints[8] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};
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
typedef struct XYStruct {
  int32_t x;
  int32_t y;
} XYStruct;
// =========================================================================================================== //
#define ModeBunker 0
#define ModePatrol 1
#define ModeAttack 2
#define ModeCruise 3
#define ModeScared 4
#define ModeEyes   5

typedef struct SpectreStruct {
// 	uint16_t x, y; // in pixels, not tile -- center pixel
  XYStruct pos;
  XYStruct dir;   // direction of movement (always -1, 0 or 1)
  XYStruct targettile;
  uint8_t  speed; // speed multlipier for direction of movement
  //uint8_t face;  // 0=Left, 1=Up, 2=Right, 3=Down
  int16_t  facing;             // Eyes Direction Facing (from 0 - TRIG_MAX_ANGLE)
  uint32_t frame; // Animation frame. 0=Skirt, 1=Skirt
  uint8_t  color;
  uint8_t  mode;
} SpectreStruct;
SpectreStruct spectre[4];

// PlayerStruct are things about the person playing
//   which are the things that stay with "player 1" and "Player 2" etc
//   including: initials, current dots remaining, score, lives, current level
typedef struct PlayerStruct {
  uint32_t score;
   uint8_t lives;
   uint8_t level;
   uint8_t dots[31];   // dots left
   uint8_t totaldots;  // current number of dots on the board
  // name/initials?
} PlayerStruct;

// MuncherStruct is about the character.
//   Properties reset between each player and/or single player's death
//   including: x,y pos, speed, mouth frame
typedef struct MuncherStruct {
// 	uint16_t x, y; // in pixels, not tile -- center pixel
  XYStruct pos;
  XYStruct dir;    // direction of movement (always -1, 0 or 1)
   uint8_t speed;  // speed multlipier for direction of movement
  //uint8_t face;  // 0=Left, 1=Up, 2=Right, 3=Down
   int16_t facing; // Eater Direction Facing (from 0 - TRIG_MAX_ANGLE)
  uint32_t frame;  // Animation frame. Mouth: 0=Closed, 1=Open, 2=Wide, 3=Open
} MuncherStruct;

// NOTE: Probably should change LevelStruct to a function which figures the data instead of a lookup table
// ... actually, make function which fills LevelStruct currentlevel
typedef struct LevelStruct {
  uint8_t bonus_sprite;
  uint16_t bonus_points;
  
  uint8_t bluemode_time;
  uint8_t bluemode_flashes;
  uint8_t eater_normal_speed;
  uint8_t eater_bluemode_speed;
  uint8_t spectre_normal_speed;
  uint8_t spectre_bluemode_speed;
  uint8_t spectre_tunnel_speed;
  uint8_t spectre_cruise1_speed;
  uint8_t spectre_cruise1_dots;
  uint8_t spectre_cruise2_speed;
  uint8_t spectre_cruise2_dots;
  
  uint8_t attackpatrol[8];
} LevelStruct;
// AttackPatrol Schedule: (all in Seconds)
// =========================================================
//  Mode       Level 1     Levels 2–4    Levels 5+
// =========================================================
// Scatter         7             7            5
//  Chase         20            20           20
// Scatter         7             7            5
//  Chase         20            20           20
// Scatter         5             5            5
//  Chase         20          1033         1037
// Scatter         5          1/60         1/60
//  Chase     indefinite   indefinite   indefinite
// =========================================================




#define AccelerometerControl   0 // Accelerometer Movement (default)
#define ULDRButtonControl      1 // Up/Down/Back/Select for Up/Down/Left/Right
#define LRButtonControl        2 // Up/Down for CounterClockwise/Clockwise rotation, Select to reverse direction

#define AccelerometerTolerance 10 // How far to tilt watch before it's recognized as joystick input
  
void update_movement_via_joystick();
void game_click_config_provider(void *context);

void   init_board();
uint8_t getmap(int32_t x, int32_t y);
void   setmap(int32_t x, int32_t y, int8_t data);
uint8_t getlevelspeed(uint8_t level);


// Make variables globally accessable
extern AccelData accel;
extern uint16_t totalpellets;
extern uint8_t animate;
//extern uint8_t speed;         // probably replace with level[currentlevel].playerspeed
//extern uint8_t *font8, *background, *font_sprites, *playersprites;

//Make all functions globally accessable
void load_graphics();
void unload_graphics();
void draw_background_ctx(GContext *ctx);
void draw_dots_ctx(GContext *ctx);
void draw_muncher_ctx(GContext *ctx);
void draw_top_ctx(GContext *ctx);

void draw_background_fb(uint8_t *fb);
void draw_dots_fb(uint8_t *fb);
void draw_muncher_fb(uint8_t *fb);
void draw_spectres(uint8_t *fb);

//void draw_font8(uint8_t *screen, int16_t x, int16_t y, uint8_t color, uint8_t chr);
void draw_sprite8(uint8_t *fb, uint8_t *font, int16_t start_x, int16_t start_y, uint8_t color, uint8_t spr);
void draw_font8_text(uint8_t *fb, int16_t x, int16_t y, uint8_t color, char *str); // str points to zero-terminated string
void draw_actor(uint8_t *fb, int32_t x, int32_t y, uint8_t color, uint8_t spr);
void draw_pupils(uint8_t *fb, int32_t x, int32_t y, uint8_t color, uint8_t facing);

void build_shadow_table();
void fill_rect(uint8_t *screen, GRect rect, uint8_t color);

void set_pattern(uint8_t *data);
void fill_framebuffer_with_pattern(uint8_t *screen, uint8_t *data);
void modify_pattern(uint8_t *data, int8_t x_offset, int8_t y_offset, uint8_t invert);
void create_pattern_layer();
void destroy_pattern_layer();


void init_player(uint8_t ID);
void create_players(uint8_t num_of_players);
void init_muncher();
void move_muncher();
void init_spectres();
void move_spectres();
void check_collisions();
void muncher_eat_dots();

//Depreciated, might bring them back
//void mainmenu();
//void intro();

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
