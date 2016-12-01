
#ifndef _HAILO_H             // prevent multiple definitions if this 
#define _HAILO_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include <ctime>
#include "textDX.h"

//=============================================================================
// Create game class
//=============================================================================
class Hailo : public Game
{
private:
	// game items	
	TextDX  *dxFontSmall;       // DirectX fonts
	TextDX  *dxFontMedium;
	TextDX  *dxFontLarge;
    // variables
	//character 
	TextureManager characterTexture;
	Image character;
	TextureManager characterWalkingTexture;
	Image characterWalking;



	TextureManager backgroundTexture;
	Image   background;
	
	TextureManager cloudTexture;
	Image cloud;
	//dropping items
	TextureManager hailTexture;
	Image hail;
	TextureManager snowTexture;
	TextureManager fastTexture;
	Image snow;
	Image snow_fast;
	Image snow_invicible;
	Image snow_minus;
	Image snow_slow;
	Image fast;
	TextureManager snow_fastTexture;
	TextureManager snow_invincibleTexture;
	TextureManager snow_minusTexture;
	TextureManager snow_slowTexture;


	//snowman
	Image snowman;
	Image snowman_head;
	TextureManager snowmanTexture;
	TextureManager snowman_headTexture;

	//state of character
	TextureManager freezeTexture;
	Image freeze;
	Image minus;
	Image	slow;
	TextureManager minusTexture;
	TextureManager	slowTexture;


	float gameTime = 0;
	float lastSnowSpawnTime = 0;
	float lastHailSpawnTime = 0;
	float lastsnow_fastSpawnTime = 0;
	float lastsnow_invincibleSpawnTime = 0;
	float lastsnow_minusSpawnTime = 0;
	float lastsnow_slowSpawnTime = 0;


	int JumpTimer = 0;
	int unFreezeTimer = 0;
	bool jumping = false;
	bool increasingYAxisJump = true;
	bool enableKey = true;
	bool stateOfUp = false;
	bool stateOfDown = false;
	int countDownKey = 0;
	bool freezeState = false;
	int buffTiming = 5000;
	int invincibleTime = 0;
	int buffState = 0;
	int velocity = 100;
	int offsetFromChar = 0;
	bool freezedMoved = false;
	//arrays
	Image snowArrayImage[20];
	Image hailArrayImage[20];
	Image snow_fastArrayImage[20];
	Image snow_invincibleArrayImage[20];
	Image snow_minusArrayImage[20];
	Image snow_slowArrayImage[20];
	int preventSameColumnSpawning[6];
	
	int p1Score;
	int p1Health = 3;

	int timer = 60;
	int elapsed_secs = 0;

	int gameStart = 0;
	clock_t begin;
	clock_t end;

public:
    // Constructor
    Hailo();

    // Destructor
    virtual ~Hailo();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
	void itemSpawn();
	boolean collisionDetection();
	void cloudAnimation();
	void characterControl();
	void frozen();
	void jumpingMethod();
	void unfreeze();
	void importImage();
	void snowAndHailArrayInitialization();
	int getNonDuplicateRanNum();
	int displayTimer();
	void checkHealth();
	void buffStateCheck();
};

#endif

