
#ifndef _HAILO_H             // prevent multiple definitions if this 
#define _HAILO_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include <ctime>

//=============================================================================
// Create game class
//=============================================================================
class Hailo : public Game
{
private:
    // variables
	TextureManager backgroundTexture;
	Image   background;
	TextureManager characterTexture;
	Image character;
	TextureManager characterWalkingTexture;
	Image characterWalking;
	TextureManager cloudTexture;
	Image cloud;
	TextureManager hailTexture;
	Image hail;
	TextureManager snowTexture;
	Image snow;
	TextureManager freezeTexture;
	Image freeze;
	Image minus;
	Image	slow;
	Image snow_fast;
	Image snow_invicible;
	Image snowman;
	Image snowman_head;
	Image snow_minus;
	Image snow_slow;
	TextureManager minusTexture;
	TextureManager	slowTexture;
	TextureManager snow_fastTexture;
	TextureManager snow_invicibleTexture;
	TextureManager snowmanTexture;
	TextureManager snowman_headTexture;
	TextureManager snow_minusTexture;
	TextureManager snow_slowTexture;


	float gameTime = 0;
	float lastSnowSpawnTime = 0;
	float lastHailSpawnTime = 0;

	int JumpTimer = 0;
	int unFreezeTimer = 0;
	bool jumping = false;
	bool increasingYAxisJump = true;
	bool enableKey = true;
	bool stateOfUp = false;
	bool stateOfDown = false;
	int countDownKey = 0;
	bool freezeState = false;
	//arrays
	Image snowArrayImage[20];
	Image hailArrayImage[20];

	int p1Score;
	
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
};

#endif

