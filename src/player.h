#pragma once
#include "global.h"

  //TODO:
  //Change PlayerStruct to be about the player
  //  which are the things that stay with "player 1" and "Player 2" etc
  // including initials, current dots remaining, score, lives, current level
  //
  // muncher properties reset between each player and/or single player death
  //   including: x,y pos, speed, mouth frame
  

void init_player(uint8_t ID);
void create_players(uint8_t num_of_players);
void add_points(uint32_t points);

//uint8_t get_current_player();
uint8_t get_lives();
uint32_t get_score();
uint8_t get_level();
