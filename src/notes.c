/*

Begin:

  BeginIntroScene(){
    Point root layer drawing to IntroDrawing()
    Point buttons to IntroButtons()
    IntroLoop()
  }
  IntroDrawing() {
    DrawBlack()
  }
  IntroButtons() {
    Back = Exit App
    Up/Dn/Sl = Stop IntroLoop Timer;  BeginMainMenuScene(SkipMenuPopup)
  }
  IntroLoop() {
    
    BeginMainMenuScene(StartMenuPopup)
    Set Timer to run IntroLoop()
  }


// at this point, the background is black
  BeginMainMenuScene(BeginMenuMode) {
    Point buttons to MainMenuButtons()
    Point root layer drawing to MainMenuDrawing()
    MainMenuLoop(MenuMode)
  }
  MainMenuButtons() {
    Up, Sel, Down = 
    If MenuMode = Establishing BG or PopIn, set MenuMode to InMenu
    Back = ExitApp()
  }
  MainMenuLoop(*data) {
  
  }
  MainMenuDrawing() {
    If MenuMode = Establishing
      Draw background however
      If background is done Set MenuMode to PopIn
    if PopIn
    
  }
Pop-In Menu()

Pop-Out Menu()

Start by pointing window draw routine to Intro

GameMode:
  0 BGpattern
  1 pop in menu
  2 menu up
  3 pop out menu
  4 

draw order:
bg pattern
then board
then closing pattern
then popup menu

Start muncher moving left
DRAWBOARD
switch(boardmode)
 ingame or normal
   draw board (normal)
   draw dots (flash dots)
   draw spectres
   draw
 playerstart(currentplayer)
   draw board
  playerready:
  
  won:
    draw board (flash color)
  dying:
    draw board
    draw dots (flashing)
    draw muncher (dying frame)
  gameover:
    draw board
    draw dots
    draw 
 

for spectre=0 to 3
If spectre state=normal
  Draw spectre as 1bit with ghostcolor
  DrawEyes(spectre.face)
else if spectre.state = dead
  DrawEyes(spectre.face)
else if spectre.state = Scare
  Draw blue spectre
end if

*/
/*
LCD LCDs:
 1  168  144
 2   84   72
 3   56   48
 4   42   36
 6   28   24
 7   24     
 8   21   18
 9        16
12   14   12
*/

// =========================================================================================================== //
//TODO:
//      Eater is sprite on screen. Has temporary variables:
//        speed
//        pos
//        facing
//        frame
//        
//      Player should be human controlling Eater and has:
//        score
//        lives
//        current level
//        which dots remain
// =========================================================================================================== //

/************************************************
  Notes
  ************************************************
  
  How positioning works:
  Each tile is 64x64
  Meaning the tile a spectre or muncher is on = position / 64 = position >> 6
   Tile  sizes are 5px x 5px
  Sprite sizes are 7px x 7px
  Whole board is 28 tiles wide x 31 tiles tall
  -- maybe change this to 28w x 36h (though 32wide, technically for wrap araound tunnels)
  
  
  
  
  Use JavaScript for Phone Storage for level layout?
  If no phone connection, use default level
  Persistent Storage for Hi-Score
  Future versions might have different game modes:
    Classic Arcade
    Item Collection (Use buttons to use items)
      - Bomb ("Eats" all ghosts)
      - Save Power Pellet for use anytime
      - Each dot = $1?
    Race the clock
    Dozens of ghosts
    multiple pebmen (Multiball)
      all controlled the same, eat each other
    Goal to eat specific tile, then board resets
    
    Begin:
    One Window
    Moving Textured Background
    Floating Window Menu:
    Title: "Pixel Muncher [Picture?]"
    Main Menu:
      Options (SL Flips to Options Menu)
      > Start / Continue (Depending on Persistant Storage)
      Players: 1/2/3/4 (SL to change)
      High Scores (SL Flips to HS Menu)
      
    While Playing:
    Accelerometer to control joystick
    Press Down to recenter joystick
    Press Back to pause/main menu
      Back again to exit
      Up/Down/Select to choose options
        Resume (default)
        Restart
        
    Back pauses/brings up floating menu
    
        
  Program Flow
  ------------
Version 1:
  1. Init
    A. Map initialized by static array
    B. Player/Ghost init positions hardcoded
    C. Init score, 1 player only
  2. Main Menu
    A. Begin Game
    B. Options
      - All Vibrations on/off
      - Death Vibration
      - "Jaws" vibration music
      - Siren vibration music
      - Sound Effects vibration
      - One/Two Player
      - Speed: 1/4x, 1/2x, 1x, 2x, 4x
    C. 
  3. Begin Game was selected
    A. Init
      - Create 2nd window
      - Init Map
      - Init Ghosts
      - Init Players
      


Version 2: (Level Editor)
  1. Init
    A. Load Map
      - Create Array
      - Read ROM, save into array
      - Draw into Bitmap (Without Dots)
    B. Init Player/Ghost positions
      - Read Map ROM for Player/Ghost positions (default facing left)
      
  
*/


// static int8_t boardlayout[MAP_W * MAP_H] =
// {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
//  -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,
//  -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1, 1,-1,
//  -1, 2,-1, 0, 0,-1, 1,-1, 0, 0, 0,-1, 1,-1,-1, 1,-1, 0, 0, 0,-1, 1,-1, 0, 0,-1, 2,-1,
//  -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1, 1,-1,
//  -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,
//  -1, 1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1, 1,-1,
//  -1, 1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1, 1,-1,
//  -1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1,-1,
//  -1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 0,-1,-1, 0,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1,-1,
//   0, 0, 0, 0, 0,-1, 1,-1,-1,-1,-1,-1, 0,-1,-1, 0,-1,-1,-1,-1,-1, 1,-1, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0,-1, 1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1,-1, 1,-1, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0,-1, 1,-1,-1, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0,-1,-1, 1,-1, 0, 0, 0, 0, 0,
//  -1,-1,-1,-1,-1,-1, 1,-1,-1, 0,-1, 0, 0, 0, 0, 0, 0,-1, 0,-1,-1, 1,-1,-1,-1,-1,-1,-1,
//   0, 0, 0, 0, 0, 0, 1, 0, 0, 0,-1, 0, 0, 0, 0, 0, 0,-1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
//  -1,-1,-1,-1,-1,-1, 1,-1,-1, 0,-1, 0, 0, 0, 0, 0, 0,-1, 0,-1,-1, 1,-1,-1,-1,-1,-1,-1,
//   0, 0, 0, 0, 0,-1, 1,-1,-1, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0,-1,-1, 1,-1, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0,-1, 1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1,-1, 1,-1, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0,-1, 1,-1,-1, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0,-1,-1, 1,-1, 0, 0, 0, 0, 0,
//  -1,-1,-1,-1,-1,-1, 1,-1,-1, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0,-1,-1, 1,-1,-1,-1,-1,-1,-1,
//  -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,
//  -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1, 1,-1,
//  -1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1, 1,-1,
//  -1, 2, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 2,-1,
//  -1,-1,-1, 1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1, 1,-1,-1,-1,
//  -1,-1,-1, 1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1, 1,-1,-1,-1,
//  -1, 1, 1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1,-1,
//  -1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,
//  -1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,
//  -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,
//  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
// };




// ------------------------------------------------------------------------ //
//  Notes
// ------------------------------------------------------------------------ //
/*
#define FONT_KEY_FONT_FALLBACK "RESOURCE_ID_FONT_FALLBACK"
#define FONT_KEY_GOTHIC_18 "RESOURCE_ID_GOTHIC_18"
#define FONT_KEY_GOTHIC_18_BOLD "RESOURCE_ID_GOTHIC_18_BOLD"
#define FONT_KEY_GOTHIC_24 "RESOURCE_ID_GOTHIC_24"
#define FONT_KEY_GOTHIC_24_BOLD "RESOURCE_ID_GOTHIC_24_BOLD"
#define FONT_KEY_GOTHIC_28 "RESOURCE_ID_GOTHIC_28"
#define FONT_KEY_GOTHIC_28_BOLD "RESOURCE_ID_GOTHIC_28_BOLD"
#define FONT_KEY_BITHAM_30_BLACK "RESOURCE_ID_BITHAM_30_BLACK"
#define FONT_KEY_BITHAM_42_BOLD "RESOURCE_ID_BITHAM_42_BOLD"
#define FONT_KEY_BITHAM_42_LIGHT "RESOURCE_ID_BITHAM_42_LIGHT"
#define FONT_KEY_BITHAM_42_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_42_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_34_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_LIGHT_SUBSET "RESOURCE_ID_BITHAM_34_LIGHT_SUBSET"
#define FONT_KEY_BITHAM_18_LIGHT_SUBSET "RESOURCE_ID_BITHAM_18_LIGHT_SUBSET"
#define FONT_KEY_ROBOTO_CONDENSED_21 "RESOURCE_ID_ROBOTO_CONDENSED_21"
#define FONT_KEY_ROBOTO_BOLD_SUBSET_49 "RESOURCE_ID_ROBOTO_BOLD_SUBSET_49"
#define FONT_KEY_DROID_SERIF_28_BOLD "RESOURCE_ID_DROID_SERIF_28_BOLD"
*/