
//  Module:             Gameplay Programming
//  Assignment1:		Hailo
//  Student Name:       Ngm Hui Min, Lois	| Jessica Tan Hwee Ching	| Pang Jin Xiang
//  Student Number:     S10158786C			| S10156827G				| S10157119D

#ifndef _HAILO_H             // prevent multiple definitions if this 
#define _HAILO_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include <ctime>
#include "textDX.h"
#include "sounds.h"

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
	//character player 2
	TextureManager characterTexture;
	Image character;
	TextureManager characterWalkingTexture;
	Image characterWalking;

	//character player 2
	TextureManager characterTexture2;
	Image character2;
	TextureManager characterWalkingTexture2;
	Image characterWalking2;

	//Background
	TextureManager backgroundTexture;
	Image   background;
	
	//start page
	TextureManager startPageTexture;
	Image   startPage;

	//end page
	TextureManager endPageTexture;
	Image   endPage;

	TextureManager cloudTexture;
	Image cloud;
	Image cloud2;

	//Dropping item
	TextureManager hailTexture;
	Image hail;
	TextureManager snowTexture;
	Image snow;

	//snow for buff
	TextureManager snow_fastTexture;
	Image snow_fast;
	TextureManager snow_invincibleTexture;
	Image snow_invicible;
	TextureManager snow_minusTexture;
	Image snow_minus;
	TextureManager snow_slowTexture;
	Image snow_slow;
	
	//snowman
	TextureManager snowmanTexture;
	Image snowman;
	TextureManager snowman_headTexture;
	Image snowman_head;

	//icon for state of character
	TextureManager freezeTexture;
	Image freeze[2];
	TextureManager minusTexture;
	Image minus[2];
	TextureManager	slowTexture;
	Image	slow[2];
	TextureManager fastTexture;
	Image fast[2];

	//spawning of the snow
	float gameTime = 0;
	float lastSnowSpawnTime = 0;
	float lastHailSpawnTime = 0;
	float lastsnow_fastSpawnTime = 0;
	float lastsnow_invincibleSpawnTime = 0;
	float lastsnow_minusSpawnTime = 0;
	float lastsnow_slowSpawnTime = 0;	

	//Jump function for player 1
	float JumpTimer = 0;
	bool jumping = false;
	bool increasingYAxisJump = true;
	bool enableKey = true;
	bool stateOfUp = false;
	bool stateOfDown = false;
	bool stateOfUpJump = false;
	bool stateOfDownJump = false;
	int countDownKey = 0;
	bool freezeState = false;
	float unfreezeTimer = 0;

	//Jump function for player 2
	float JumpTimer2 = 0;
	bool jumping2 = false;
	bool increasingYAxisJump2 = true;
	bool enableKey2 = true;
	bool stateOfUp2 = false;
	bool stateOfDown2 = false;
	int countDownKey2 = 0;
	bool freezeState2 = false;
	float unfreezeTimer2 = 0;

	//buffing state time for Player 1
	int buffTiming = 5;
	float buffForEffectTime = 0;
	int invincibleTime = 0;
	int buffState = 0;
	int velocity = 200;


	//buffing state time for Player 2
	int buffTiming2 = 5;
	float buffForEffectTime2 = 0;
	int invincibleTime2 = 0;
	int buffState2 = 0;
	int velocity2 = 200;


	bool invincibleDoNotIgnore = true;
	//arrays for snow
	Image snowArrayImage[20];
	Image hailArrayImage[20];
	Image snow_fastArrayImage[20];
	Image snow_invincibleArrayImage[20];
	Image snow_minusArrayImage[20];
	Image snow_slowArrayImage[20];
	int SpawnColumn[6];
	int SpawnTime[6];
	int SpawnDropSpeed[6];
	bool repeat = true;
	bool rollingOfDeath = false;
	int direction;

	//health and score for player 1
	int p1Score;
	int p1Health = 3;

	//health and score for player 2
	int p2Score;
	int p2Health = 3;

	int timer = 60;
	int elapsed_secs = 0;

	int gameStart = 0;
	clock_t begin;
	clock_t end;

	// sounds
	Sounds* sounds;

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
	boolean collisionDetection(Image c, Image cw, int playerNum);
	void cloudAnimation();
	void characterControl();
	void characterControl2();
	void frozen();
	void frozen2();
	void jumpingMethod();
	void unfreeze(int playerNum, Image c);
	void importImage();
	void snowAndHailArrayInitialization();
	int displayTimer();
	void checkHealth();
	void buffStateCheck(Image c, Image cw, int playerNum);
	void jumpingMethod2();
	bool checkingCollision(Image image1, Image image2);
	void rollingSnowBallOfDeath();
	int randomNum(int from, int to);
	float RandomFloat(float a, float b);
};
#endif

