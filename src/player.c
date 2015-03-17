#include <pebble.h>
#include "player.h"

// encapsulate by making all variables static
  
typedef struct PlayerStruct {
  uint32_t score;
   uint8_t lives;
   uint8_t level;
  // dots[]
  // name/initials?
} PlayerStruct;

static PlayerStruct player[1];
static uint8_t current_player;
static uint8_t max_players;

uint8_t get_next_player() {
  //for players current+1 mod max to max: if lives>0 return it
  //for players = 1 to current, if lives>0 return it
  //all players have 0 lives, if so, return 255
  //set_current_player()
 return 0;
}

void create_players(uint8_t num_of_players) {
  if(num_of_players>9) num_of_players=9;  // Stick with single digits (1-9, no player 0)
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

void init_player(uint8_t ID) {
  player[ID].score=0;
  player[ID].level=1;
  player[ID].lives=3;
  //player dots on board
  //name="Player " & Number
  
}

void add_points(uint32_t points) {player[current_player].score+=points;}
uint32_t get_score() {return player[current_player].score;}
 uint8_t get_lives() {return player[current_player].lives;}
 uint8_t get_level() {return player[current_player].level;}
 uint8_t get_current_player() {return current_player;}
//bool player_is_dead(
//void set_current_player(uint8_t ID) {current_player = ID;}