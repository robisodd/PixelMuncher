#include <pebble.h>
#include "muncher.h"
#include "map.h"
#include "player.h"
  
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
    add_points(10);//    player[currentplayer].score += 10;                                   // add 10 points to score
    setmap(muncher.pos.x, muncher.pos.y, 0); // remove dot from board
  }
  if(getmap(muncher.pos.x, muncher.pos.y)==2) {  // if on a super dot, eat superdot
    muncher.speed -= 6;                                                   // slow down due to eating superdot
    add_points(50); //player[currentplayer].score += 50;                                   // add 50 points to score
    setmap(muncher.pos.x, muncher.pos.y, 0); // remove superdot from board
    // [TODO] start CHASE mode
  }

}