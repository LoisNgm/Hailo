
#include "hailo.h"
#include <iostream>
#include <time.h> 
using namespace std;

//=============================================================================
// Constructor
//=============================================================================
Hailo::Hailo()
{
	dxFontSmall = new TextDX();     // DirectX fonts
	dxFontMedium = new TextDX();
	dxFontLarge = new TextDX();
}

//=============================================================================
// Destructor
//=============================================================================
Hailo::~Hailo()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Hailo::initialize(HWND hwnd)
{
	
	Game::initialize(hwnd); // throws GameError
	
	importImage();
	snowAndHailArrayInitialization();
	

	//snowman
	snowman.setX(GAME_WIDTH / 2 - snowman.getWidth()/2);
	snowman.setY(GAME_HEIGHT - snowman.getHeight() - 160.0f);
	snowman.setCurrentFrame(SNOWMAN_START_FRAME);
	snowman.setFrames(SNOWMAN_START_FRAME, SNOWMAN_END_FRAME);
	snowman.setFrameDelay(SNOWMAN_ANIMATION_DELAY);
	freeze.setVisible(false);

	//characters
	snow.setY(30);
	snow.setVisible(false);
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++)
	{
		snowArrayImage[i] = snow;
	}
	hail.setY(30);
	hail.setVisible(false);
	for (int i = 0; i < (sizeof(hailArrayImage) / sizeof(Image)); i++)
	{
		hailArrayImage[i] = hail;
	}

	hail.setY(30);
	hail.setVisible(false);
	

	freeze.setVisible(false);

	character.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
	characterWalking.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
	characterWalking.setCurrentFrame(CHARACTERWALKING_START_FRAME);
	characterWalking.setFrames(CHARACTERWALKING_START_FRAME,CHARACTERWALKING_END_FRAME);
	characterWalking.setFrameDelay(CHARACTERWALKING_ANIMATION_DELAY);
	characterWalking.setVisible(false);
	character.setVisible(true);
	
	// 15 pixel high Arial
	if (dxFontSmall->initialize(graphics, 15, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	// 62 pixel high Arial
	if (dxFontMedium->initialize(graphics, 62, true, false, "Calibri") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	// 124 pixel high Arial
	if (dxFontLarge->initialize(graphics, 124, true, false, "Calibri") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	if (dxFont.initialize(graphics, gameNS::POINT_SIZE, false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize DirectX font."));
	
    return;
}
int counting = 0;
//=============================================================================
// Update all game items
//=============================================================================
void Hailo::update()
{
	itemSpawn();
	//cloud's animation....
	cloudAnimation();

	//if else to prevent keyDown when freezing characters
	if (enableKey)
	{
		characterControl();
	}
	else
	{
		frozen();
	}
	characterWalking.update(frameTime);
	snowman.update(frameTime);
	buffStateCheck();
	jumpingMethod();
	checkHealth();
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Hailo::ai()
{
	
}

//=============================================================================
// Handle collisions
//=============================================================================
void Hailo::collisions()
{
	collisionDetection();
}

//=============================================================================
// Render game items
//=============================================================================
void Hailo::render()
{
	graphics->spriteBegin();                // begin drawing sprites
	background.draw();
	
	snow.draw();
	hail.draw();
	cloud.draw();
	snow_fast.draw();
	snow_invicible.draw();
	snowman.draw();
	snowman_head.draw();
	snow_minus.draw();
	snow_slow.draw();

	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++)
	{
		snowArrayImage[i].draw();
	}
	for (int i = 0; i < (sizeof(hailArrayImage) / sizeof(Image)); i++)
	{
		hailArrayImage[i].draw();
	}
	for (int i = 0; i < (sizeof(snow_fastArrayImage) / sizeof(Image)); i++)
	{
		snow_fastArrayImage[i].draw();
	}
	for (int i = 0; i < (sizeof(snow_invincibleArrayImage) / sizeof(Image)); i++)
	{
		snow_invincibleArrayImage[i].draw();
	}
	for (int i = 0; i < (sizeof(snow_minusArrayImage) / sizeof(Image)); i++)
	{
		snow_minusArrayImage[i].draw();
	}
	for (int i = 0; i < (sizeof(snow_slowArrayImage) / sizeof(Image)); i++)
	{
		snow_slowArrayImage[i].draw();
	}
	character.draw();
	characterWalking.draw();
	freeze.draw();
	minus.draw();
	slow.draw();
	fast.draw();
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];
	
	//dxFontSmall->setFontColor(graphicsNS::BLACK);
	//dxFontMedium->setFontColor(graphicsNS::BLACK);
	dxFontMedium->setFontColor(graphicsNS::WHITE);
	dxFontMedium->print(to_string(displayTimer()), 500, GAME_HEIGHT - 100);
	//dxFontLarge->print("C", 20, 100);
	//dxFontMedium->print("C", 114, 148);
	//dxFontSmall->print("C", 164, 184);
	dxFont.setFontColor(gameNS::FONT_COLOR);
	// convert score to Cstring
	
	_snprintf_s(buffer, BUF_SIZE, "P1\nScore: %d", (int)p1Score);
	dxFont.print(buffer, 100, GAME_HEIGHT - 100);
	//_snprintf_s(buffer, BUF_SIZE, "P2\nScore: %d\n Health: %d");
	//dxFont.print(buffer, GAME_WIDTH - 200, GAME_HEIGHT - 100);
	
	graphics->spriteEnd();


}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Hailo::releaseAll()
{
	backgroundTexture.onLostDevice();
	characterTexture.onLostDevice();
	cloudTexture.onLostDevice();
	snowTexture.onLostDevice();
	hailTexture.onLostDevice();
	characterWalkingTexture.onLostDevice();
	freezeTexture.onLostDevice();
	minusTexture.onLostDevice();
	slowTexture.onLostDevice();
	snow_fastTexture.onLostDevice();
	snow_invincibleTexture.onLostDevice();
	snowmanTexture.onLostDevice();
	snowman_headTexture.onLostDevice();
	snow_minusTexture.onLostDevice();
	snow_slowTexture.onLostDevice();
	fastTexture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Hailo::resetAll()
{
	backgroundTexture.onResetDevice();
	characterTexture.onResetDevice();
	cloudTexture.onResetDevice();
	snowTexture.onResetDevice();
	hailTexture.onResetDevice();
	characterWalkingTexture.onResetDevice();
	freezeTexture.onResetDevice();
	minusTexture.onResetDevice();
	slowTexture.onResetDevice();
	snow_fastTexture.onResetDevice();
	snow_invincibleTexture.onResetDevice();
	snowmanTexture.onResetDevice();
	snowman_headTexture.onResetDevice();
	snow_minusTexture.onResetDevice();
	snow_slowTexture.onResetDevice();
	fastTexture.onResetDevice();
    Game::resetAll();
    return;
}
//spawning hail and snow
void Hailo::itemSpawn()
{
	gameTime += frameTime;//using time from game class
	srand(time(0));
	int randomOnType = rand() % 6 + 1;
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snowArrayImage[i].setDegrees(snowArrayImage[i].getDegrees() + SNOW_ROTATION_RATE);// snow rotating
		if (snowArrayImage[i].getVisible())
		{

			snowArrayImage[i].setY(snowArrayImage[i].getY() + frameTime * SNOW_SPEED);//snow travelling downwards

			if (snowArrayImage[i].getY() > FLOOR)// checking for item out of screen
			{
				snowArrayImage[i].setY(30);//reset snow position
				snowArrayImage[i].setVisible(false);//reuse snow object.
			}
		}
		else
		{
			if ((gameTime - lastSnowSpawnTime) > SPAWNTIME)
			{
				snowArrayImage[i].setX((rand() % 20 + 1) * (GAME_WIDTH / 20) + snowArrayImage[i].getWidth() / 2);
				snowArrayImage[i].setVisible(true);//spawn snow
				lastSnowSpawnTime = gameTime;//reset spawn time(r)
				//cout << "Snow: " << snowArrayImage[i].getX() << " , " << snowArrayImage[i].getY() << endl;
				break;
			}
		}
	}
	for (int i = 0; i < (sizeof(hailArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		hailArrayImage[i].setDegrees(hailArrayImage[i].getDegrees() + SNOW_ROTATION_RATE);// hail rotating
		if (hailArrayImage[i].getVisible())
		{

			hailArrayImage[i].setY(hailArrayImage[i].getY() + frameTime * SNOW_SPEED);//hail travelling downwards

			if (hailArrayImage[i].getY() > FLOOR)// checking for item out of screen
			{
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
			}

		}
		else
		{
			if ((gameTime - lastHailSpawnTime) > SPAWNTIME)
			{
				hailArrayImage[i].setX((rand() % 20 + 1) * (GAME_WIDTH / 20) + hailArrayImage[i].getWidth() / 2);
				hailArrayImage[i].setVisible(true);//spawn hail
				lastHailSpawnTime = gameTime;//reset spawn time(r)
				//cout << "Hail: " << hailArrayImage[i].getX() << " , " << hailArrayImage[i].getY() << endl;
				break;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_fastArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_fastArrayImage[i].setDegrees(snow_fastArrayImage[i].getDegrees() + SNOW_ROTATION_RATE);// snow_fast rotating
		if (snow_fastArrayImage[i].getVisible())
		{

			snow_fastArrayImage[i].setY(snow_fastArrayImage[i].getY() + frameTime * SNOW_SPEED);//snow_fast travelling downwards

			if (snow_fastArrayImage[i].getY() > FLOOR)// checking for item out of screen
			{
				snow_fastArrayImage[i].setY(30);//reset snow_fast position
				snow_fastArrayImage[i].setVisible(false);//reuse snow_fast object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_fastSpawnTime) > SPAWNTIME)
			{
				snow_fastArrayImage[i].setX((rand() % 20 + 1) * (GAME_WIDTH / 20) + snow_fastArrayImage[i].getWidth() / 2);
				snow_fastArrayImage[i].setVisible(true);//spawn snow_fast
				lastsnow_fastSpawnTime = gameTime;//reset spawn time(r)
				//cout << "snow_fast: " << snow_fastArrayImage[i].getX() << " , " << snow_fastArrayImage[i].getY() << endl;
				break;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_invincibleArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_invincibleArrayImage[i].setDegrees(snow_invincibleArrayImage[i].getDegrees() + SNOW_ROTATION_RATE);// snow_invincible rotating
		if (snow_invincibleArrayImage[i].getVisible())
		{

			snow_invincibleArrayImage[i].setY(snow_invincibleArrayImage[i].getY() + frameTime * SNOW_SPEED);//snow_invincible travelling downwards

			if (snow_invincibleArrayImage[i].getY() > FLOOR)// checking for item out of screen
			{
				snow_invincibleArrayImage[i].setY(30);//reset snow_invincible position
				snow_invincibleArrayImage[i].setVisible(false);//reuse snow_invincible object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_invincibleSpawnTime) > SPAWNTIME)
			{
				snow_invincibleArrayImage[i].setX((rand() % 20 + 1) * (GAME_WIDTH / 20) + snow_invincibleArrayImage[i].getWidth() / 2);
				snow_invincibleArrayImage[i].setVisible(true);//spawn snow_invincible
				lastsnow_invincibleSpawnTime = gameTime;//reset spawn time(r)
				//cout << "snow_invincible: " << snow_invincibleArrayImage[i].getX() << " , " << snow_invincibleArrayImage[i].getY() << endl;
				break;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_minusArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_minusArrayImage[i].setDegrees(snow_minusArrayImage[i].getDegrees() + SNOW_ROTATION_RATE);// snow_minus rotating
		if (snow_minusArrayImage[i].getVisible())
		{

			snow_minusArrayImage[i].setY(snow_minusArrayImage[i].getY() + frameTime * SNOW_SPEED);//snow_minus travelling downwards

			if (snow_minusArrayImage[i].getY() > FLOOR)// checking for item out of screen
			{
				snow_minusArrayImage[i].setY(30);//reset snow_minus position
				snow_minusArrayImage[i].setVisible(false);//reuse snow_minus object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_minusSpawnTime) > SPAWNTIME)
			{
				snow_minusArrayImage[i].setX((rand() % 20 + 1) * (GAME_WIDTH / 20) + snow_minusArrayImage[i].getWidth() / 2);
				snow_minusArrayImage[i].setVisible(true);//spawn snow_minus
				lastsnow_minusSpawnTime = gameTime;//reset spawn time(r)
				//cout << "snow_minus: " << snow_minusArrayImage[i].getX() << " , " << snow_minusArrayImage[i].getY() << endl;
				break;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_slowArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_slowArrayImage[i].setDegrees(snow_slowArrayImage[i].getDegrees() + SNOW_ROTATION_RATE);// snow_slow rotating
		if (snow_slowArrayImage[i].getVisible())
		{

			snow_slowArrayImage[i].setY(snow_slowArrayImage[i].getY() + frameTime * SNOW_SPEED);//snow_slow travelling downwards

			if (snow_slowArrayImage[i].getY() > FLOOR)// checking for item out of screen
			{
				snow_slowArrayImage[i].setY(30);//reset snow_slow position
				snow_slowArrayImage[i].setVisible(false);//reuse snow_slow object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_slowSpawnTime) > SPAWNTIME)
			{
				snow_slowArrayImage[i].setX((rand() % 20 + 1) * (GAME_WIDTH / 20) + snow_slowArrayImage[i].getWidth() / 2);
				snow_slowArrayImage[i].setVisible(true);//spawn snow_slow
				lastsnow_slowSpawnTime = gameTime;//reset spawn time(r)
				//cout << "snow_slow: " << snow_slowArrayImage[i].getX() << " , " << snow_slowArrayImage[i].getY() << endl;
				break;
			}
		}
	}
	for (int i = 0; i < (sizeof(preventSameColumnSpawning) / sizeof(int)); i++)
	{
		preventSameColumnSpawning[i] = 0;
	}
}
boolean Hailo::collisionDetection()
{
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++){
		// character and snow
		if ((character.getX() + character.getWidth() - 20) >= (snowArrayImage[i].getX()) &&
			(character.getX() + 15) <= (snowArrayImage[i].getX() + snowArrayImage[i].getWidth()) &&
			(character.getY() + character.getHeight()) >= (snowArrayImage[i].getY()) &&
			(character.getY() + 10) <= (snowArrayImage[i].getY() + snowArrayImage[i].getHeight())){
			if (buffState==4 )
			{
				p1Score -= (rand() % 100 + 1);
			}
			else
			{
				p1Score += (rand() % 100 + 1);

			}
			snowArrayImage[i].setY(30);//reset snow position
			snowArrayImage[i].setVisible(false);//reuse snow object.
			return true;
		}
		// character walking and snow
		if ((characterWalking.getX() + characterWalking.getWidth() - 20) >= (snowArrayImage[i].getX()) &&
			(characterWalking.getX() + 15) <= (snowArrayImage[i].getX() + snowArrayImage[i].getWidth()) &&
			(characterWalking.getY() + characterWalking.getHeight()) >= (snowArrayImage[i].getY()) &&
			(characterWalking.getY() + 10) <= (snowArrayImage[i].getY() + snowArrayImage[i].getHeight())){
			if (buffState ==4)
			{
				p1Score -= (rand() % 100 + 1);
			}
			else
			{
				p1Score += (rand() % 100 + 1);
			}
			snowArrayImage[i].setY(30);//reset snow position
			snowArrayImage[i].setVisible(false);//reuse snow object.
			return true;
		}
		if (buffState != 2)
		{
			// character and hail
			if ((character.getX() + character.getWidth() - 20) >= (hailArrayImage[i].getX()) &&
				(character.getX() + 15) <= (hailArrayImage[i].getX() + hailArrayImage[i].getWidth()) &&
				(character.getY() + character.getHeight()) >= (hailArrayImage[i].getY()) &&
				(character.getY() + 10) <= (hailArrayImage[i].getY() + hailArrayImage[i].getHeight())){
				p1Score -= rand() % 51 + 50;
				p1Health--;
				enableKey = false;
				characterWalking.setVisible(false);
				character.setVisible(true);
				freezeState = true;
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
				return true;
			}
			// character walking and hail
			if ((characterWalking.getX() + characterWalking.getWidth() - 20) >= (hailArrayImage[i].getX()) &&
				(characterWalking.getX() + 15) <= (hailArrayImage[i].getX() + hailArrayImage[i].getWidth()) &&
				(characterWalking.getY() + characterWalking.getHeight()) >= (hailArrayImage[i].getY()) &&
				(characterWalking.getY() + 10) <= (hailArrayImage[i].getY() + hailArrayImage[i].getHeight())){
				p1Score -= rand() % 51 + 50;
				p1Health--;
				enableKey = false;
				characterWalking.setVisible(false);
				character.setVisible(true);
				freezeState = true;
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
				return true;
			}
			//Colliding with downgrades
			//Character and slow speed snow
			if ((character.getX() + character.getWidth() - 20) >= (snow_slowArrayImage[i].getX()) &&
				(character.getX() + 15) <= (snow_slowArrayImage[i].getX() + snow_slowArrayImage[i].getWidth()) &&
				(character.getY() + character.getHeight()) >= (snow_slowArrayImage[i].getY()) &&
				(character.getY() + 10) <= (snow_slowArrayImage[i].getY() + snow_slowArrayImage[i].getHeight()))
			{
				characterWalking.setVisible(false);
				character.setVisible(true);
				snow_slowArrayImage[i].setY(30);
				snow_slowArrayImage[i].setVisible(false);
				buffTiming = 5000;
				buffState = 3;
				velocity = 50;
				fast.setVisible(false);
				minus.setVisible(false);
				buffTiming = 5;
				return true;
			}
			// character walking and slow speed snow 
			if ((characterWalking.getX() + characterWalking.getWidth() - 20) >= (snow_slowArrayImage[i].getX()) &&
				(characterWalking.getX() + 15) <= (snow_slowArrayImage[i].getX() + snow_slowArrayImage[i].getWidth()) &&
				(characterWalking.getY() + characterWalking.getHeight()) >= (snow_slowArrayImage[i].getY()) &&
				(characterWalking.getY() + 10) <= (snow_slowArrayImage[i].getY() + snow_slowArrayImage[i].getHeight())){
				characterWalking.setVisible(false);
				character.setVisible(true);
				snow_slowArrayImage[i].setY(30);
				snow_slowArrayImage[i].setVisible(false);
				buffTiming = 5000;
				fast.setVisible(false);
				minus.setVisible(false);
				buffState = 3;
				velocity = 50;
				buffTiming = 5;
				return true;
			}

			//Colliding with downgrades
			//Character and minus snowball
			if ((character.getX() + character.getWidth() - 20) >= (snow_minusArrayImage[i].getX()) &&
				(character.getX() + 15) <= (snow_minusArrayImage[i].getX() + snow_minusArrayImage[i].getWidth()) &&
				(character.getY() + character.getHeight()) >= (snow_minusArrayImage[i].getY()) &&
				(character.getY() + 10) <= (snow_minusArrayImage[i].getY() + snow_minusArrayImage[i].getHeight())){
				characterWalking.setVisible(false);
				character.setVisible(true);
				snow_minusArrayImage[i].setY(30);
				snow_minusArrayImage[i].setVisible(false);
				slow.setVisible(false);
				fast.setVisible(false);
				minus.setVisible(true);
				buffState = 4;
				buffTiming = 5;
				return true;
			}
			// character walking and minus snowball
			if ((characterWalking.getX() + characterWalking.getWidth() - 20) >= (snow_minusArrayImage[i].getX()) &&
				(characterWalking.getX() + 15) <= (snow_minusArrayImage[i].getX() + snow_minusArrayImage[i].getWidth()) &&
				(characterWalking.getY() + characterWalking.getHeight()) >= (snow_minusArrayImage[i].getY()) &&
				(characterWalking.getY() + 10) <= (snow_minusArrayImage[i].getY() + snow_minusArrayImage[i].getHeight())){
				characterWalking.setVisible(false);
				character.setVisible(true);
				snow_minusArrayImage[i].setY(30);
				snow_minusArrayImage[i].setVisible(false);
				slow.setVisible(false);
				fast.setVisible(false);
				minus.setVisible(true);
				buffState = 4;
				buffTiming = 5;
				return true;
			}
		}
		//Colliding with Upgrades
		//character and invincible snow
		if ((character.getX() + character.getWidth() - 20) >= (snow_invincibleArrayImage[i].getX()) &&
			(character.getX() + 15) <= (snow_invincibleArrayImage[i].getX() + snow_invincibleArrayImage[i].getWidth()) &&
			(character.getY() + character.getHeight()) >= (snow_invincibleArrayImage[i].getY()) &&
			(character.getY() + 10) <= (snow_invincibleArrayImage[i].getY() + snow_invincibleArrayImage[i].getHeight())){
			characterWalking.setVisible(false);
			character.setVisible(true);
			snow_invincibleArrayImage[i].setY(30);//reset invincible position
			snow_invincibleArrayImage[i].setVisible(false);//reuse invincible object.
			buffState = 2;
			buffTiming = 5000;
			velocity = 100;
			slow.setVisible(false);
			fast.setVisible(false);
			minus.setVisible(false);
			buffTiming = 5;
			return true;
		}
		// character walking and invincible
		if ((characterWalking.getX() + characterWalking.getWidth() - 20) >= (snow_invincibleArrayImage[i].getX()) &&
			(characterWalking.getX() + 15) <= (snow_invincibleArrayImage[i].getX() + snow_invincibleArrayImage[i].getWidth()) &&
			(characterWalking.getY() + characterWalking.getHeight()) >= (snow_invincibleArrayImage[i].getY()) &&
			(characterWalking.getY() + 10) <= (snow_invincibleArrayImage[i].getY() + snow_invincibleArrayImage[i].getHeight())){
			characterWalking.setVisible(false);
			character.setVisible(true);
			snow_invincibleArrayImage[i].setY(30);
			snow_invincibleArrayImage[i].setVisible(false);
			buffState = 2;
			buffTiming = 5000;
			velocity = 100;
			slow.setVisible(false);
			minus.setVisible(false);
			fast.setVisible(false);
			buffTiming = 5;
			return true;
		}

		// character and speed increase snowball
		if ((character.getX() + character.getWidth() - 20) >= (snow_fastArrayImage[i].getX()) &&
			(character.getX() + 15) <= (snow_fastArrayImage[i].getX() + snow_fastArrayImage[i].getWidth()) &&
			(character.getY() + character.getHeight()) >= (snow_fastArrayImage[i].getY()) &&
			(character.getY() + 10) <= (snow_fastArrayImage[i].getY() + snow_fastArrayImage[i].getHeight())){
			characterWalking.setVisible(false);
			character.setVisible(true);
			snow_fastArrayImage[i].setY(30);
			snow_fastArrayImage[i].setVisible(false);
			buffTiming = 5000;
			buffState = 1;
			velocity = 200;
			buffTiming = 5;
			slow.setVisible(false);
			fast.setVisible(true);
			minus.setVisible(false);
			return true;
		}
		// character walking and speed increase snowball
		if ((characterWalking.getX() + characterWalking.getWidth() - 20) >= (snow_fastArrayImage[i].getX()) &&
			(characterWalking.getX() + 15) <= (snow_fastArrayImage[i].getX() + snow_fastArrayImage[i].getWidth()) &&
			(characterWalking.getY() + characterWalking.getHeight()) >= (snow_fastArrayImage[i].getY()) &&
			(characterWalking.getY() + 10) <= (snow_fastArrayImage[i].getY() + snow_fastArrayImage[i].getHeight())){
			characterWalking.setVisible(false);
			character.setVisible(true);
			snow_fastArrayImage[i].setY(30);
			snow_fastArrayImage[i].setVisible(false);
			buffTiming = 5000;
			buffState = 1;
			velocity = 200;
			buffTiming = 5;
			slow.setVisible(false);
			fast.setVisible(true);
			minus.setVisible(false);
			return true;
		}
	}
	return false;	
}
void Hailo::cloudAnimation()
{
	//cloud's animation....
	if (cloud.getX() > GAME_WIDTH) // position off screen left
	{
		cloud.setX((float)-cloud.getWidth());
	}
	else
	{
		cloud.setX(cloud.getX() + frameTime * CHARACTER_SPEED);
	}
}

void Hailo::characterControl()
{
	//character controls...
	characterWalking.update(frameTime);
	if (input->isKeyDown(VK_RIGHT))            // if move right
	{

		if (character.getX() > GAME_WIDTH - character.getWidth())               // if off screen right
		{

		}
		else
		{
			character.setVisible(false);
			character.setX(character.getX() + frameTime * velocity);
			characterWalking.flipHorizontal(false);
			characterWalking.setX(character.getX() + frameTime * velocity);
			// for testing collision
			/*if (collisionDetection()){
			characterWalking.setVisible(false);
			}
			else
			{
			characterWalking.setVisible(true);
			}*/
			characterWalking.setVisible(true);
			characterWalking.update(frameTime);

			// testing character's x-coordinates
			//cout << "Character: " << characterWalking.getX() << endl;
		}

	}
	else{
		character.setVisible(true);
		characterWalking.setVisible(false);
	}
	if (input->isKeyDown(VK_LEFT))             // if move left
	{

		if (character.getX() < 0)         // if off screen left
		{
		}
		else
		{
			character.setVisible(false);
			character.setX(character.getX() - frameTime * velocity);
			characterWalking.flipHorizontal(true);
			characterWalking.setX(character.getX() - frameTime * velocity);
			// for testing collision
			/*if (collisionDetection()){
			characterWalking.setVisible(false);
			}
			else
			{
			characterWalking.setVisible(true);
			}*/
			characterWalking.setVisible(true);
			characterWalking.update(frameTime);
		}
	}
	if (input->wasKeyPressed(VK_UP))             // if jump
	{
		jumping = true;					//to trigger jump
		input->clearKeyPress(VK_UP);
	}
	else
	{
		characterWalking.update(frameTime);
	}
	if (input->isKeyDown(VK_UP))             // if jump
	{
		jumping = true;					//to trigger jump
	}
	else
		characterWalking.update(frameTime);
}

//when character is hit by hail, this method checks for the frozen state
void Hailo::frozen()
{
	freeze.setY(character.getY() + character.getHeight() / 4);
	//to check if the freeze image x needs an update
	if (freezedMoved==false)
	{
		freeze.setX(character.getX());
	}
	freeze.setVisible(freezeState);
	if (freezeState && character.getY() == 498)
	{
		freezedMoved = true;
		//When it first collide with the character, 
		//check if the freeze image is on the character
		if (offsetFromChar == 0)
			freeze.setX(character.getX());
		unfreeze();
	}
	
	
}
/*Description:Allow the character to jump*/
void Hailo::jumpingMethod()
{
	if (jumping)
	{
		if (increasingYAxisJump)
		{
			if (JumpTimer < 150)
			{
				JumpTimer += 3;
				//increase Y axis
				character.setY(character.getY() - frameTime * CHARACTER_JUMP_SPEED);
				characterWalking.setY(characterWalking.getY() - frameTime * CHARACTER_JUMP_SPEED);
			}
			else
			{
				if (character.getY() <371 || character.getY() >371 || characterWalking.getY()<371 || characterWalking.getY()>371)
				{
					character.setY(371);
					characterWalking.setY(371);
				}
				else
				{
					increasingYAxisJump = false;
				}
			}
		}
		else
		{
			if (JumpTimer > 0)
			{
				JumpTimer -= 3;
				character.setY(character.getY() + frameTime * CHARACTER_JUMP_SPEED);
				characterWalking.setY(characterWalking.getY() + frameTime * CHARACTER_JUMP_SPEED);
			}
			else
			{
				if (character.getY() > 498 || character.getY() < 498)
				{
					character.setY(498);
				}
				jumping = false;
				increasingYAxisJump = true;
			}
		}
	}
}

/*Description:When the character is frozen, 
time limit and the number of times the character
press down is recorded. The number of times the down is pressed is to speed up the freezing state*/
void Hailo::unfreeze()
{

	if (input->isKeyDown(VK_DOWN))
	{
		stateOfDown = true;		
		freeze.setX(character.getX() + 20);
	}
	if (!input->isKeyDown(VK_DOWN) && stateOfDown == true)
	{
		stateOfUp = true;		
		freeze.setX(character.getX() - 20 );
	}
	if (stateOfUp)
	{

		stateOfDown = false;
		stateOfUp = false;
		countDownKey++;
		unFreezeTimer += 100;//to unfreeze faster	
	}

	unFreezeTimer++;
	if (countDownKey >= 20 || unFreezeTimer >= 2000)//around 10 seconds or key was pressed and released 20 or more times
	{
		enableKey = true;
		countDownKey = 0;
		unFreezeTimer = 0;
		freezeState = false;
		freezedMoved = false;
		offsetFromChar = 0;
		freeze.setVisible(freezeState);
	}

}
void Hailo::importImage()
{
	//character type Image

	// characterWalking texture
	if (!characterWalkingTexture.initialize(graphics, CHARACTERWALKING_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character walking texture"));

	// characterWalking
	if (!characterWalking.initialize(graphics, CHARACTERWALKING_WIDTH, CHARACTERWALKING_HEIGHT, CHARACTERWALKING_COLS, &characterWalkingTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	// character texture
	if (!characterTexture.initialize(graphics, CHARACTER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	// character
	if (!character.initialize(graphics, CHARACTER_WIDTH, CHARACTER_HEIGHT, CHARACTER_COLS, &characterTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	//dropping from the sky

	// hail texture
	if (!hailTexture.initialize(graphics, HAIL_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hail texture"));

	// hail
	if (!hail.initialize(graphics, 0, 0, 0, &hailTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	// snow texture
	if (!snowTexture.initialize(graphics, SNOW_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snow texture"));

	// snow
	if (!snow.initialize(graphics, 0, 0, 0, &snowTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	// snow_fast texture
	if (!snow_fastTexture.initialize(graphics, SNOW_FAST_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snow_fast texture"));

	// snow_fast
	if (!snow_fast.initialize(graphics, 0, 0, 0, &snow_fastTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	
	// snow_invinicible texture
	if (!snow_invincibleTexture.initialize(graphics, SNOW_INVINCIBLE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snow_invincible texture"));

	// snow_invinicible
	if (!snow_invicible.initialize(graphics, 0, 0, 0, &snow_invincibleTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	
	// snow_minus texture
	if (!snow_minusTexture.initialize(graphics, SNOW_MINUS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_minus texture"));

	// snow_minus
	if (!snow_minus.initialize(graphics, 0, 0, 0, &snow_minusTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	
	// snow_slow texture
	if (!snow_slowTexture.initialize(graphics, SNOW_SLOW_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_minus texture"));

	// snow_slow
	if (!snow_slow.initialize(graphics, 0, 0, 0, &snow_slowTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));



	// snowman texture
	if (!snowmanTexture.initialize(graphics, SNOWMAN_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman texture"));

	// snowman_head texture
	if (!snowman_headTexture.initialize(graphics, SNOWMAN_HEAD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_minus texture"));

	// snowman_head texture
	if (!snowman_headTexture.initialize(graphics, SNOWMAN_HEAD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_minus texture"));

	
	//character effect type
	// freeze texture
	if (!freezeTexture.initialize(graphics, FREEZE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing freeze texture"));

	// freeze
	if (!freeze.initialize(graphics, 0, 0, 0, &freezeTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	// minus texture
	if (!minusTexture.initialize(graphics, MINUS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing minus texture"));

	// minus
	if (!minus.initialize(graphics, 0, 0, 0, &minusTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// slow texture
	if (!slowTexture.initialize(graphics, SLOW_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing slow texture"));

	// slow
	if (!slow.initialize(graphics, 0, 0, 0, &slowTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	
	//snowman parts

	// snowman
	if (!snowman.initialize(graphics, SNOWMAN_WIDTH, SNOWMAN_HEIGHT, SNOWMAN_COLS, &snowmanTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// snowman_head texture
	if (!snowman_headTexture.initialize(graphics, SNOWMAN_HEAD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_head texture"));

	// snowman_head
	if (!snowman_head.initialize(graphics, 0, 0, 0, &snowman_headTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	
	// snowman_head
	if (!snowman_head.initialize(graphics, 0, 0, 0, &snowman_headTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	
	//background type
	// background texture
	if (!backgroundTexture.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// background
	if (!background.initialize(graphics, 0, 0, 0, &backgroundTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	// cloud texture
	if (!cloudTexture.initialize(graphics, CLOUD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cloud texture"));

	// cloud
	if (!cloud.initialize(graphics, 0, 0, 0, &cloudTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));


	// fast texture
	if (!fastTexture.initialize(graphics, FAST_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing fast texture"));

	// fast
	if (!fast.initialize(graphics, 0, 0, 0, &fastTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
}

void Hailo::snowAndHailArrayInitialization()
{
	//snow's and hail array and settings
	snow.setY(30);
	snow.setVisible(false);
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++)
	{
		snowArrayImage[i] = snow;
	}
	hail.setY(30);
	hail.setVisible(false);
	for (int i = 0; i < (sizeof(hailArrayImage) / sizeof(Image)); i++)
	{
		hailArrayImage[i] = hail;
	}
	snow_fast.setY(30);
	snow_fast.setVisible(false);
	for (int i = 0; i < (sizeof(snow_fastArrayImage) / sizeof(Image)); i++)
	{
		snow_fastArrayImage[i] = snow_fast;
	}
	snow_invicible.setY(30);
	snow_invicible.setVisible(false);
	for (int i = 0; i < (sizeof(snow_invincibleArrayImage) / sizeof(Image)); i++)
	{
		snow_invincibleArrayImage[i] = snow_invicible;
	}
	snow_minus.setY(30);
	snow_minus.setVisible(false);
	for (int i = 0; i < (sizeof(snow_minusArrayImage) / sizeof(Image)); i++)
	{
		snow_minusArrayImage[i] = snow_minus;
	}
	snow_slow.setY(30);
	snow_slow.setVisible(false);
	for (int i = 0; i < (sizeof(snow_slowArrayImage) / sizeof(Image)); i++)
	{
		snow_slowArrayImage[i] = snow_slow;
	}
}

int Hailo::getNonDuplicateRanNum()
{
	int tempRan = 0;
	bool repeat = true;
	//srand(time(0));
	do
	{
		tempRan = (rand() % 20 + 1);
		for (int i = 0; i < (sizeof(preventSameColumnSpawning) / sizeof(int)) - 1; i++)
		{
			if (preventSameColumnSpawning[i] != 0)
			{
				
				if (preventSameColumnSpawning[i] == tempRan)
				{
					repeat = true;
					cout << "Duplicate drop detected." << endl;
					break;
				}
				else
				{
					repeat = false;
					preventSameColumnSpawning[i+1] = tempRan;
					cout << "Random: " << tempRan << endl;
					return tempRan;
				}
			}
			else
			{
			    getNonDuplicateRanNum();
				preventSameColumnSpawning[i] = tempRan;
				return tempRan;
			}
		}
	} while (repeat);	
}
void Hailo::buffStateCheck()
{
	if (buffState == 1)//fast speed
	{
		fast.setX(character.getCenterX() - fast.getWidth() / 2);
		fast.setY(character.getCenterY() - fast.getHeight() / 2);
		if ((gameTime - buffForEffectTime) > BUFFTIME && buffTiming > 0)
		{
			buffTiming--;
			fast.update(frameTime);
			buffForEffectTime = gameTime;
		}
		if (buffTiming <= 0)
		{
			buffState = 0;
			velocity = 100;
			fast.setVisible(false);
			buffTiming = 5;
		}
	}
	else if (buffState == 2)
	{
		//animation
		if (invincibleTime <= 2)
		{
			character.setVisible(false);
			characterWalking.setVisible(false);
		}
		else
		{
			if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
				characterWalking.setVisible(true);
			else
				character.setVisible(true);
		}
		if (invincibleTime >= 4)
			invincibleTime = 0;
		invincibleTime++;
		if ((gameTime - buffForEffectTime)>BUFFTIME&&buffTiming > 0)
		{	
			buffTiming--;
			buffForEffectTime = gameTime;
		}	
		if (buffTiming <= 0)
		{
			if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
			{
				characterWalking.setVisible(true);
			}
			else
			{
				character.setVisible(true);
			}
			buffState = 0;
			invincibleTime = 1000;
			buffTiming = 5;
		}
	}
	else if (buffState == 3)//slow speed
	{
		slow.setX(character.getCenterX() - slow.getWidth() / 2);
		slow.setY(character.getCenterY() - slow.getHeight() / 2);
		if ((gameTime - buffForEffectTime)>BUFFTIME&&buffTiming > 0)
		{			
			buffTiming--;
			slow.setVisible(true);
			buffForEffectTime = gameTime;
			slow.update(frameTime);
		}
		if (buffTiming <= 0)
		{
			slow.setVisible(false);
			buffState = 0;
			velocity = 100;
			buffTiming = 5;
		}
	}
	else if (buffState == 4)//minus points
	{
		minus.setX(character.getCenterX() - minus.getWidth() / 2);
		minus.setY(character.getCenterY() - minus.getHeight() / 2);
		if ((gameTime - buffForEffectTime)>BUFFTIME&&buffTiming > 0)
		{		
			buffTiming--;
			minus.setVisible(true);			
			buffForEffectTime = gameTime;
			minus.update(frameTime);						
		}
		if (buffTiming<=0)
		{
			minus.setVisible(false);
			buffState = 0;			
			buffTiming = 5;
		}
	}
}

void Hailo::checkHealth(){
	if (p1Health <= 0)
		cout << "game end" << endl;
}
int Hailo::displayTimer(){
	if (timer - elapsed_secs > 0){
		end = clock();
		elapsed_secs = int(end - begin) / CLOCKS_PER_SEC;
		return timer - elapsed_secs;
		
	}
	else{
		paused = true;
		return 0;
	
		
	}
}