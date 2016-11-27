
#ifndef _CONSTANTS_H            // prevent multiple definitions if this 
#define _CONSTANTS_H            // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

//-----------------------------------------------
//                  Constants
//-----------------------------------------------
// graphic images
const char BACKGROUND_IMAGE[] = "pictures\\background.png";  // background
const char CHARACTER_IMAGE[] = "pictures\\character_white_resting.png";//character
const char CHARACTERWALKING_IMAGE[] = "pictures\\character_white_walking.png";//characterWALKING
const char CLOUD_IMAGE[] = "pictures\\clouds.png";//cloud
const char HAIL_IMAGE[] = "pictures\\Hail.png";//hail
const char SNOW_IMAGE[] = "pictures\\snow.png";//snow

// window
const char CLASS_NAME[] = "Hailo";
const char GAME_TITLE[] = "Hailo";
const bool FULLSCREEN = false;             // windowed or fullscreen
const UINT GAME_WIDTH =  1028;               // width of game in pixels
const UINT GAME_HEIGHT = 768;               // height of game in pixels
const int ObjectDeopPosition[19] = { 10, 50, 90, 130, 170, 210, 250, 290, 330, 370, 410, 450, 490, 530, 570, 610, 650, 730 };

 
// game
const double PI = 3.14159265;
const float FRAME_RATE  = 200.0f;               // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float SNOW_SPAWNTIME = 2.0f;

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key



//character variable
const float CHARACTER_SPEED = 100.0f;

//character 
const int  CHARACTER_START_FRAME = 0;         // starting frame of CHARACTER animation
const int  CHARACTER_END_FRAME = 3;           // last frame of CHARACTER animation
const float CHARACTER_ANIMATION_DELAY = 0.2f; // time between frames of CHARACTER animation
const int  CHARACTER_COLS = 4;                // CHARACTER texture has 1 columns
const int  CHARACTER_WIDTH = 80;              // width of CHARACTER image
const int  CHARACTER_HEIGHT = 112;             // height of CHARACTER image

//characterWalking
const int  CHARACTERWALKING_START_FRAME = 0;         // starting frame of CHARACTERWALKING animation
const int  CHARACTERWALKING_END_FRAME = 9;           // last frame of CHARACTERWALKING animation
const float CHARACTERWALKING_ANIMATION_DELAY = 0.3f; // time between frames of CHARACTERWALKING animation
const int  CHARACTERWALKING_COLS = 9;                // CHARACTERWALKING texture has 1 columns
const int  CHARACTERWALKING_WIDTH = 80;              // width of CHARACTERWALKING image
const int  CHARACTERWALKING_HEIGHT = 112;             // height of CHARACTERWALKING image






#endif
