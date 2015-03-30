#include <pebble.h>
#include "global.h"
Layer *title_layer;
Layer *main_menu_layer;
Layer *options_menu_layer;
Layer *high_scores_layer;
#define shadowheight 4
extern Window *main_window;
extern Layer *root_layer;
extern Layer *pattern_layer;
AppTimer *menutimer;


bool PoppingIn=false;
int16_t popin_speed;

//#define UPDATE_MS2 1000
uint32_t UPDATE_MS2=50;
void Pop_In(Layer *me) {
  if(PoppingIn) {
    if(popin_speed<0 && me->bounds.origin.x<0) {   // done popping in
      PoppingIn = false;
      popin_speed = -1 * me->bounds.origin.x;  // put back to proper spot when addition happens below
      UPDATE_MS2=100;
    } else {                                       // still popping in
      popin_speed -= 1;  // apply deceleration
    }
     me->bounds.origin.x += popin_speed*1;
     me->frame.origin.x  -= popin_speed*2;
     me->frame.size.w    += popin_speed*4;
  } else {                                         // init popin
    popin_speed = 9;
    me->bounds.origin.x -= (me->frame.size.w/4) +  (7*1);// - (popin_speed*1);//30;
    me->frame.origin.x  += (me->frame.size.w/2) +  (7*2);// - (popin_speed*2);//60;
    me->frame.size.w    -= (me->frame.size.w/1) +  (7*4);// - (popin_speed*4);//120;
    PoppingIn = true;
  }
}

static void menuloop(void *data) {
  if(PoppingIn) Pop_In(main_menu_layer);
//   menutimer = app_timer_register(UPDATE_MS, menuloop, NULL);
  menutimer = app_timer_register(UPDATE_MS2, menuloop, NULL);
  layer_mark_dirty(root_layer);  // Schedule redraw of screen
}

// ------------------------------------------------------------------------ //
//  Drawing
// ------------------------------------------------------------------------ //
static void draw_frame(Layer *me, GContext *ctx) {
  //     me->clips = false;
  GRect rect;
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {   // if successfully captured the framebuffer
    uint8_t* screen = gbitmap_get_data(framebuffer);

    rect=me->frame;
    rect.origin.x += shadowheight;
    rect.origin.y += shadowheight;
    fill_rect(screen, rect, 0b01000000);

    rect=me->frame;
    #ifdef PBL_COLOR
      fill_rect(screen, rect, 0b01000011);
    #else
      fill_rect(screen, rect, 0b11000000);
    #endif
    graphics_release_frame_buffer(ctx, framebuffer);
  }  // endif successfully captured framebuffer

  graphics_context_set_stroke_color(ctx, GColorWhite);
  rect=me->frame;
  rect.origin.x = 1;
  rect.origin.y = 1;
  rect.size.h  -= 2;
  rect.size.w  -= 2;
//   graphics_draw_rect(ctx, rect);
}

static void draw_main_menu_layer(Layer *me, GContext *ctx) {
  draw_frame(me, ctx);

 
  GRect textframe;
  textframe = me->bounds;
  textframe.origin.x += 8;
  textframe.origin.y += 8;
  static char text[100];  //Buffer to hold text
  //textframe = GRect(11, 10, 120, 144);  // Text Box Position and Size: x, y, w, h
  snprintf(text, sizeof(text), "Options\n> Start Game!\nPlayers: 1\nHigh Scores");  // What text to draw
  graphics_context_set_text_color(ctx, GColorBlack);  // Text Color
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), textframe, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  
  textframe.origin.x -= 3;
  textframe.origin.y -= 3;
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), textframe, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text

  
}

static void options_menu_layer_update(Layer *me, GContext *ctx) {
  draw_frame(me, ctx);
}

static void title_layer_update(Layer *me, GContext *ctx) {
  draw_frame(me, ctx);
  
  static char text[100];
  GRect textframe = me->bounds; //textframe = GRect(11, 10, 120, 144);  // Text Box Position and Size: x, y, w, h
  snprintf(text, sizeof(text), "b(%d, %d, %d, %d)\nf(%d, %d, %d, %d)",
          main_menu_layer->bounds.origin.x,  main_menu_layer->bounds.origin.y,  main_menu_layer->bounds.size.w,  main_menu_layer->bounds.size.h,
          main_menu_layer->frame.origin.x,  main_menu_layer->frame.origin.y,  main_menu_layer->frame.size.w,  main_menu_layer->frame.size.h
          );
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14), textframe, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);  //Write Text
  

  GRect rect = GRect(5,5,4,100);
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {   // if successfully captured the framebuffer
    uint8_t* screen = gbitmap_get_data(framebuffer);
    fill_rect(screen, rect, 0b01000000);
    graphics_release_frame_buffer(ctx, framebuffer);
  }  // endif successfully captured framebuffer

}



// ------------------------------------------------------------------------ //
//  Button Pushing
// ------------------------------------------------------------------------ //
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  main_menu_layer->bounds.origin.x += 1;
  main_menu_layer->frame.origin.x -= 2;
  main_menu_layer->frame.size.w += 4;
}
static void dn_click_handler(ClickRecognizerRef recognizer, void *context) {
  main_menu_layer->bounds.origin.x -= 1;
  main_menu_layer->frame.origin.x += 2;
  main_menu_layer->frame.size.w -= 4;
  
}
static void sl_click_handler(ClickRecognizerRef recognizer, void *context) {
  Pop_In(main_menu_layer);
}

static void bk_click_handler(ClickRecognizerRef recognizer, void *context) {
//   if(current_menu = main_menu)
    window_stack_pop_all(false);
  //else
    //switch_to(main_menu);
}

static void main_menu_button_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, dn_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, sl_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, bk_click_handler);
}


static void create_menu_layers() {
  title_layer = layer_create(GRect(12, 10, 120, 30));
  layer_set_update_proc(title_layer, title_layer_update);
  layer_add_child(root_layer, title_layer);

  main_menu_layer = layer_create(GRect(22, 50, 100, 100));
  layer_set_update_proc(main_menu_layer, draw_main_menu_layer);
  layer_add_child(root_layer, main_menu_layer);
  
  options_menu_layer = layer_create(GRect(12, 50, 120, 100));
  layer_set_update_proc(options_menu_layer, options_menu_layer_update);
  layer_add_child(root_layer, options_menu_layer);
  layer_set_hidden(options_menu_layer, true); // hidden

  Pop_In(main_menu_layer);
}

void mainmenu() {
  layer_set_hidden(pattern_layer, true);
  create_menu_layers();
  window_set_click_config_provider(main_window, main_menu_button_config);
  menutimer = app_timer_register(UPDATE_MS2, menuloop, NULL);
}


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