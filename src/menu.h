#pragma once
#include "global.h"
#include "render.h"
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

/*

    Main Menu:
    UP/DN = Cursor Change
    BK = Exit Program
      Options (SL Flips to Options Menu)
      > Start / Continue (Depending on Persistant Storage)
      Players: 1/2/3/4 (SL to change)
      High Scores (SL Flips to HS Menu)

    Options Menu:
    UP/DN = Cursor Change
    BK = Flips back to [PREVIOUS MENU]
      Vibration ON/OFF  (SL = Toggle)
      Backlight ON/AUTO (SL = Toggle)
      Erase High Scores (SL = Flips to AreYouSure Menu)
      
    High Scores Menu:
    UP/DN = scrolls
    SL/BK = Back to Main Menu
      Shows Top 10 scores and 3-letter Names
      
      

    Paused Menu:
    UP/DN = Cursor Change
      Options (SL Flips to Paused Options Menu)
      > Continue
      

void EstablishBG(*data){
  if data isn't NULL, it's a pointer to the next function to call
    Init
    save poiner as NextPointer
    exit
  If it is NULL:
    If first run:
      Init buttons
      Init variables
    else
      Do Stuff
      If done
        point to NextPointer
      else
        Schedule timer to Loop(NULL)
}
Begin:
// at this point, the background is black
  BeginMainMenuScene(BeginMenuMode) {
    Point buttons to MainMenuButtons()
    Point root layer drawing to MainMenuDrawing()
    MenuMode=EstablishingBG
    CursorPos=2
    Menu=MainMenu
    MainMenuLoop(MenuMode)
  }
  MainMenuButtons() {
    Up, Down = 
      If MenuMode = EstablishingBG or PopIn, set MenuMode to InMenu
      If MenuMode = InMenu: CursorUp/Dn
    Sel = 
      If MenuMode = EstablishingBG or PopIn, set MenuMode to InMenu
      If MenuMode = InMenu
        If Menu = MainMenu
          If CursorPos = 1: FlipTo(OptionMenu)
          If CursorPos = 2: PopOut
          
    Back = 
      if Menu = MainMenu or PausedMenu
        ExitApp()
      if Menu = HighScores
        Menu = MainMenu
      if Menu = OptionsMenu
        Menu = MainMenu
      if Menu = PausedOptionsMenu
        Menu = PausedMenu
        
  }
  MainMenuLoop(*data) {
  
  }
  MainMenuDrawing() {
    If MenuMode = Establishing
      Draw background however
      If background is done Set MenuMode to PopIn
    if PopIn
    Set Timer for MainMenuLoop
  }
Pop-In Menu()

Pop-Out Menu()


*/
/*

Begin Main Menu Mode:
0=Began Normally
1=Begin but Skip Straight to menu
2=Paused
*/
  
// void BeginMainMenuScene(uint8_t BeginMenuMode) {
//     Point buttons to MainMenuButtons()
//     Point root layer drawing to MainMenuDrawing()
//     MainMenuLoop(MenuMode)
//   }