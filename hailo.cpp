
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

	sounds = 0;
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
	

	freeze[0].setVisible(false);
	freeze[1].setVisible(false);
	//Character player 1
	character.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
	characterWalking.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
	characterWalking.setCurrentFrame(CHARACTERWALKING_START_FRAME);
	characterWalking.setFrames(CHARACTERWALKING_START_FRAME,CHARACTERWALKING_END_FRAME);
	characterWalking.setFrameDelay(CHARACTERWALKING_ANIMATION_DELAY);
	characterWalking.setVisible(false);
	character.setVisible(true);
	
	//Character player 2
	character2.setY(GAME_HEIGHT - character2.getHeight() - 160.0f);
	characterWalking2.setY(GAME_HEIGHT - character2.getHeight() - 160.0f);
	characterWalking2.setCurrentFrame(CHARACTERWALKING_START_FRAME);
	characterWalking2.setFrames(CHARACTERWALKING_START_FRAME, CHARACTERWALKING_END_FRAME);
	characterWalking2.setFrameDelay(CHARACTERWALKING_ANIMATION_DELAY);
	characterWalking2.setVisible(false);
	character2.setVisible(true);


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

	// Create the sound object.
	sounds = new Sounds;
	if (!sounds)
	{
		return;
	}

	// Initialize the sound object.
	bool result = sounds->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct Sound.", "Error", MB_OK);
		return;
	}
    return;
}
//=============================================================================
// Update all game items
//=============================================================================
void Hailo::update()
{
	itemSpawn();
	//cloud's animation....
	cloudAnimation();
	characterWalking.update(frameTime);
	characterWalking2.update(frameTime);
	snowman.update(frameTime);
	buffStateCheck(character, characterWalking, 1);
	buffStateCheck(character2, characterWalking2, 2);
	jumpingMethod();
	jumpingMethod2();
	checkHealth();
	//if else to prevent keyDown when freezing characters
	if (enableKey)
	{
		characterControl();
	}
	else
	{
		frozen(character,characterWalking,1);
	}
	
	if (enableKey2)
	{
		characterControl2();
	}
	else
	{
		frozen(character2,characterWalking2, 2);
	}

	
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
	collisionDetection(character,characterWalking,1);
	collisionDetection(character2, characterWalking2,2);
}

//=============================================================================
// Render game items
//=============================================================================
void Hailo::render()
{
	graphics->spriteBegin();                // begin drawing sprites
	if (gameStart == 0){
		startPage.draw();
		if (input->anyKeyPressed()){
			begin = clock();
			gameStart = 1;
			paused = false;
		}
	}
	else if (gameStart == 1){
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
		character2.draw();
		characterWalking2.draw();
		freeze[0].draw();
		minus[0].draw();
		slow[0].draw();
		fast[0].draw();
		freeze[1].draw();
		minus[1].draw();
		slow[1].draw();
		fast[1].draw();
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

		_snprintf_s(buffer, BUF_SIZE, "P1\nHealth: %d", (int)p1Health);
		dxFont.print(buffer, 100, GAME_HEIGHT - 100);
		//_snprintf_s(buffer, BUF_SIZE, "P2\nScore: %d\n Health: %d");
		//dxFont.print(buffer, GAME_WIDTH - 200, GAME_HEIGHT - 100);
	}
	if (gameStart == 3)
	{
		deleteAll();
		initialize(hwnd);
	}
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
	startPageTexture.onLostDevice();
	fastTexture.onLostDevice();
	// Release the sound object.
	if (sounds)
	{
		sounds->Shutdown();
		delete sounds;
		sounds = 0;
	}
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
	fastTexture.onResetDevice();
	snow_fastTexture.onResetDevice();
	snow_invincibleTexture.onResetDevice();
	snowmanTexture.onResetDevice();
	snowman_headTexture.onResetDevice();
	snow_minusTexture.onResetDevice();
	snow_slowTexture.onResetDevice();
	startPageTexture.onResetDevice();
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
boolean Hailo::collisionDetection(Image c, Image cw, int playerNum)
{
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++){
		// c and snow
		if ((c.getX() + c.getWidth() - 20) >= (snowArrayImage[i].getX()) &&
			(c.getX() + 15) <= (snowArrayImage[i].getX() + snowArrayImage[i].getWidth()) &&
			(c.getY() + c.getHeight()) >= (snowArrayImage[i].getY()) &&
			(c.getY() + 10) <= (snowArrayImage[i].getY() + snowArrayImage[i].getHeight())){
			if (buffState == 4)
			{
				p1Score -= (rand() % 100 + 1);
			}
			else
			{
				p1Score += (rand() % 100 + 1);
			}
			if (buffState2 == 4)
			{
				p2Score -= (rand() % 100 + 1);
			}
			else
			{
				p2Score += (rand() % 100 + 1);
			}

			snowArrayImage[i].setY(30);//reset snow position
			snowArrayImage[i].setVisible(false);//reuse snow object.
			PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
			return true;
		}
		// c walking and snow
		if ((cw.getX() + cw.getWidth() - 20) >= (snowArrayImage[i].getX()) &&
			(cw.getX() + 15) <= (snowArrayImage[i].getX() + snowArrayImage[i].getWidth()) &&
			(cw.getY() + cw.getHeight()) >= (snowArrayImage[i].getY()) &&
			(cw.getY() + 10) <= (snowArrayImage[i].getY() + snowArrayImage[i].getHeight())){
			if (buffState == 4)
			{
				p1Score -= (rand() % 100 + 1);
			}
			else
			{
				p1Score += (rand() % 100 + 1);
			}
			if (buffState2 == 4)
			{
				p2Score -= (rand() % 100 + 1);
			}
			else
			{
				p2Score += (rand() % 100 + 1);
			}
			snowArrayImage[i].setY(30);//reset snow position
			snowArrayImage[i].setVisible(false);//reuse snow object.
			PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
			return true;
		}
		if (buffState != 2&& playerNum==1) 		
		{
			// c and hail
			if ((c.getX() + c.getWidth() - 20) >= (hailArrayImage[i].getX()) &&
				(c.getX() + 15) <= (hailArrayImage[i].getX() + hailArrayImage[i].getWidth()) &&
				(c.getY() + c.getHeight()) >= (hailArrayImage[i].getY()) &&
				(c.getY() + 10) <= (hailArrayImage[i].getY() + hailArrayImage[i].getHeight())){
					p1Score -= rand() % 51 + 50;
					p1Health--;
					enableKey = false;
					freezeState = true;				
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
				PlaySound(TEXT("sounds\\hail.wav"), NULL, SND_ASYNC);
				return true;
			}
			// c walking and hail
			if ((cw.getX() + cw.getWidth() - 20) >= (hailArrayImage[i].getX()) &&
				(cw.getX() + 15) <= (hailArrayImage[i].getX() + hailArrayImage[i].getWidth()) &&
				(cw.getY() + cw.getHeight()) >= (hailArrayImage[i].getY()) &&
				(cw.getY() + 10) <= (hailArrayImage[i].getY() + hailArrayImage[i].getHeight())){
				if (playerNum == 1)
				{
					p1Score -= rand() % 51 + 50;
					p1Health--;
					enableKey = false;
					freezeState = true;
					cw.setVisible(false);
					c.setVisible(true);
				}
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
				PlaySound(TEXT("sounds\\hail.wav"), NULL, SND_ASYNC);
				return true;
			}
			//Colliding with downgrades
			//c and slow speed snow
			if ((c.getX() + c.getWidth() - 20) >= (snow_slowArrayImage[i].getX()) &&
				(c.getX() + 15) <= (snow_slowArrayImage[i].getX() + snow_slowArrayImage[i].getWidth()) &&
				(c.getY() + c.getHeight()) >= (snow_slowArrayImage[i].getY()) &&
				(c.getY() + 10) <= (snow_slowArrayImage[i].getY() + snow_slowArrayImage[i].getHeight()))
			{
				cw.setVisible(false);
				c.setVisible(true);
				snow_slowArrayImage[i].setY(30);
				snow_slowArrayImage[i].setVisible(false);
				if (playerNum == 1)
				{
					buffTiming = 5000;
					buffState = 3;
					velocity = 50;
					buffTiming = 5;
				}
				fast[playerNum - 1].setVisible(false);
				minus[playerNum-1].setVisible(false);
				cw.setVisible(false);
				c.setVisible(true);
				PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
				return true;
			}
			// c walking and slow speed snow 
			if ((cw.getX() + cw.getWidth() - 20) >= (snow_slowArrayImage[i].getX()) &&
				(cw.getX() + 15) <= (snow_slowArrayImage[i].getX() + snow_slowArrayImage[i].getWidth()) &&
				(cw.getY() + cw.getHeight()) >= (snow_slowArrayImage[i].getY()) &&
				(cw.getY() + 10) <= (snow_slowArrayImage[i].getY() + snow_slowArrayImage[i].getHeight())){

				if (playerNum == 1)
				{
					buffTiming = 5000;
					buffState = 3;
					velocity = 50;
					buffTiming = 5;
				}
				cw.setVisible(false);
				c.setVisible(true);
				snow_slowArrayImage[i].setY(30);
				snow_slowArrayImage[i].setVisible(false);
				fast[playerNum - 1].setVisible(false);
				minus[playerNum - 1].setVisible(false);
				PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
				return true;
			}

			//Colliding with downgrades
			//c and minus snowball
			if ((c.getX() + c.getWidth() - 20) >= (snow_minusArrayImage[i].getX()) &&
				(c.getX() + 15) <= (snow_minusArrayImage[i].getX() + snow_minusArrayImage[i].getWidth()) &&
				(c.getY() + c.getHeight()) >= (snow_minusArrayImage[i].getY()) &&
				(c.getY() + 10) <= (snow_minusArrayImage[i].getY() + snow_minusArrayImage[i].getHeight())){
				cw.setVisible(false);
				c.setVisible(true);
				snow_minusArrayImage[i].setY(30);
				snow_minusArrayImage[i].setVisible(false);
				slow[playerNum - 1].setVisible(false);
				fast[playerNum - 1].setVisible(false);
				minus[playerNum - 1].setVisible(true);
				if (playerNum == 1)
				{
					buffState = 4;
					buffTiming = 5;
				}
				PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
				return true;
			}
			// c walking and minus snowball
			if ((cw.getX() + cw.getWidth() - 20) >= (snow_minusArrayImage[i].getX()) &&
				(cw.getX() + 15) <= (snow_minusArrayImage[i].getX() + snow_minusArrayImage[i].getWidth()) &&
				(cw.getY() + cw.getHeight()) >= (snow_minusArrayImage[i].getY()) &&
				(cw.getY() + 10) <= (snow_minusArrayImage[i].getY() + snow_minusArrayImage[i].getHeight())){
				cw.setVisible(false);
				c.setVisible(true);
				snow_minusArrayImage[i].setY(30);
				snow_minusArrayImage[i].setVisible(false);
				slow[playerNum - 1].setVisible(false);
				fast[playerNum - 1].setVisible(false);
				minus[playerNum - 1].setVisible(true);
				if (playerNum == 1)
				{
					buffState = 4;
					buffTiming = 5;
				}
				return true;
			}
		}
		if (buffState2 != 2 && playerNum==2)
		{
			// c and hail
			if ((c.getX() + c.getWidth() - 20) >= (hailArrayImage[i].getX()) &&
				(c.getX() + 15) <= (hailArrayImage[i].getX() + hailArrayImage[i].getWidth()) &&
				(c.getY() + c.getHeight()) >= (hailArrayImage[i].getY()) &&
				(c.getY() + 10) <= (hailArrayImage[i].getY() + hailArrayImage[i].getHeight())){		
					p2Score -= rand() % 51 + 50;
					p2Health--;
					enableKey2 = false;
					freezeState2 = true;
					cw.setVisible(false);
					c.setVisible(true);
				
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
				return true;
			}
			// c walking and hail
			if ((cw.getX() + cw.getWidth() - 20) >= (hailArrayImage[i].getX()) &&
				(cw.getX() + 15) <= (hailArrayImage[i].getX() + hailArrayImage[i].getWidth()) &&
				(cw.getY() + cw.getHeight()) >= (hailArrayImage[i].getY()) &&
				(cw.getY() + 10) <= (hailArrayImage[i].getY() + hailArrayImage[i].getHeight())){
					p2Score -= rand() % 51 + 50;
					p2Health--;
					enableKey2 = false;
					freezeState2 = true;
					cw.setVisible(false);
					c.setVisible(true);
				
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
				return true;
			}
			//Colliding with downgrades
			//c and slow speed snow
			if ((c.getX() + c.getWidth() - 20) >= (snow_slowArrayImage[i].getX()) &&
				(c.getX() + 15) <= (snow_slowArrayImage[i].getX() + snow_slowArrayImage[i].getWidth()) &&
				(c.getY() + c.getHeight()) >= (snow_slowArrayImage[i].getY()) &&
				(c.getY() + 10) <= (snow_slowArrayImage[i].getY() + snow_slowArrayImage[i].getHeight()))
			{
				cw.setVisible(false);
				c.setVisible(true);
				snow_slowArrayImage[i].setY(30);
				snow_slowArrayImage[i].setVisible(false);
				
					buffTiming2 = 5000;
					buffState2 = 3;
					velocity2 = 50;
					buffTiming = 5;
			
				fast[playerNum - 1].setVisible(false);
				minus[playerNum - 1].setVisible(false);
				cw.setVisible(false);
				c.setVisible(true);

				return true;
			}
			// c walking and slow speed snow 
			if ((cw.getX() + cw.getWidth() - 20) >= (snow_slowArrayImage[i].getX()) &&
				(cw.getX() + 15) <= (snow_slowArrayImage[i].getX() + snow_slowArrayImage[i].getWidth()) &&
				(cw.getY() + cw.getHeight()) >= (snow_slowArrayImage[i].getY()) &&
				(cw.getY() + 10) <= (snow_slowArrayImage[i].getY() + snow_slowArrayImage[i].getHeight())){			
					buffTiming2 = 5000;
					buffState2 = 3;
					velocity2 = 50;
					buffTiming = 5;				
				cw.setVisible(false);
				c.setVisible(true);
				snow_slowArrayImage[i].setY(30);
				snow_slowArrayImage[i].setVisible(false);
				fast[playerNum - 1].setVisible(false);
				minus[playerNum - 1].setVisible(false);
				return true;
			}

			//Colliding with downgrades
			//c and minus snowball
			if ((c.getX() + c.getWidth() - 20) >= (snow_minusArrayImage[i].getX()) &&
				(c.getX() + 15) <= (snow_minusArrayImage[i].getX() + snow_minusArrayImage[i].getWidth()) &&
				(c.getY() + c.getHeight()) >= (snow_minusArrayImage[i].getY()) &&
				(c.getY() + 10) <= (snow_minusArrayImage[i].getY() + snow_minusArrayImage[i].getHeight())){
				cw.setVisible(false);
				c.setVisible(true);
				snow_minusArrayImage[i].setY(30);
				snow_minusArrayImage[i].setVisible(false);
				slow[playerNum - 1].setVisible(false);
				fast[playerNum - 1].setVisible(false);
				minus[playerNum - 1].setVisible(true);				
				buffState2 = 4;
				buffTiming2 = 5;				
				return true;
			}
			// c walking and minus snowball
			if ((cw.getX() + cw.getWidth() - 20) >= (snow_minusArrayImage[i].getX()) &&
				(cw.getX() + 15) <= (snow_minusArrayImage[i].getX() + snow_minusArrayImage[i].getWidth()) &&
				(cw.getY() + cw.getHeight()) >= (snow_minusArrayImage[i].getY()) &&
				(cw.getY() + 10) <= (snow_minusArrayImage[i].getY() + snow_minusArrayImage[i].getHeight())){
				cw.setVisible(false);
				c.setVisible(true);
				snow_minusArrayImage[i].setY(30);
				snow_minusArrayImage[i].setVisible(false);
				slow[playerNum - 1].setVisible(false);
				fast[playerNum - 1].setVisible(false);
				minus[playerNum - 1].setVisible(true);			
				buffState2 = 4;
				buffTiming2 = 5;
				PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
				return true;
			}
		}
		//Colliding with Upgrades
		//c and invincible snow
		if ((c.getX() + c.getWidth() - 20) >= (snow_invincibleArrayImage[i].getX()) &&
			(c.getX() + 15) <= (snow_invincibleArrayImage[i].getX() + snow_invincibleArrayImage[i].getWidth()) &&
			(c.getY() + c.getHeight()) >= (snow_invincibleArrayImage[i].getY()) &&
			(c.getY() + 10) <= (snow_invincibleArrayImage[i].getY() + snow_invincibleArrayImage[i].getHeight())){
			cw.setVisible(false);
			c.setVisible(true);
			snow_invincibleArrayImage[i].setY(30);//reset invincible position
			snow_invincibleArrayImage[i].setVisible(false);//reuse invincible object.
			if (playerNum==1)
			{
				buffState = 2;
				buffTiming = 5000;
				velocity = 100;
				buffTiming = 5;
			}
			else
			{
				buffState2 = 2;
				buffTiming2 = 5000;
				velocity2 = 100;
				buffTiming2 = 5;
			}
			slow[playerNum - 1].setVisible(false);
			fast[playerNum - 1].setVisible(false);
			minus[playerNum - 1].setVisible(false);
			PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
			return true;
		}
		// c walking and invincible
		if ((cw.getX() + cw.getWidth() - 20) >= (snow_invincibleArrayImage[i].getX()) &&
			(cw.getX() + 15) <= (snow_invincibleArrayImage[i].getX() + snow_invincibleArrayImage[i].getWidth()) &&
			(cw.getY() + cw.getHeight()) >= (snow_invincibleArrayImage[i].getY()) &&
			(cw.getY() + 10) <= (snow_invincibleArrayImage[i].getY() + snow_invincibleArrayImage[i].getHeight())){
			cw.setVisible(false);
			c.setVisible(true);
			if (playerNum == 1)
			{
				buffState = 2;
				buffTiming = 5000;
				velocity = 100;
				buffTiming = 5;
			}
			else
			{
				buffState2 = 2;
				buffTiming2 = 5000;
				velocity2 = 100;
				buffTiming2 = 5;
			}
			snow_invincibleArrayImage[i].setY(30);
			snow_invincibleArrayImage[i].setVisible(false);
			slow[playerNum - 1].setVisible(false);
			minus[playerNum - 1].setVisible(false);
			fast[playerNum - 1].setVisible(false);
			PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
			return true;
		}

		// c and speed increase snowball
		if ((c.getX() + c.getWidth() - 20) >= (snow_fastArrayImage[i].getX()) &&
			(c.getX() + 15) <= (snow_fastArrayImage[i].getX() + snow_fastArrayImage[i].getWidth()) &&
			(c.getY() + c.getHeight()) >= (snow_fastArrayImage[i].getY()) &&
			(c.getY() + 10) <= (snow_fastArrayImage[i].getY() + snow_fastArrayImage[i].getHeight())){
			cw.setVisible(false);
			c.setVisible(true);
			if (playerNum == 1)
			{
				buffTiming = 5000;
				buffState = 1;
				velocity = 200;
				buffTiming = 5;
			}
			else
			{
				buffTiming2 = 5000;
				buffState2 = 1;
				velocity2 = 200;
				buffTiming2 = 5;
			}

			snow_fastArrayImage[i].setY(30);
			snow_fastArrayImage[i].setVisible(false);
			slow[playerNum - 1].setVisible(false);
			fast[playerNum - 1].setVisible(true);
			minus[playerNum - 1].setVisible(false);
			PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
			return true;
		}
		// c walking and speed increase snowball
		if ((cw.getX() + cw.getWidth() - 20) >= (snow_fastArrayImage[i].getX()) &&
			(cw.getX() + 15) <= (snow_fastArrayImage[i].getX() + snow_fastArrayImage[i].getWidth()) &&
			(cw.getY() + cw.getHeight()) >= (snow_fastArrayImage[i].getY()) &&
			(cw.getY() + 10) <= (snow_fastArrayImage[i].getY() + snow_fastArrayImage[i].getHeight())){
			if (playerNum == 1)
			{
				buffTiming = 5000;
				buffState = 1;
				velocity = 200;
				buffTiming = 5;
			}
			else
			{
				buffTiming2 = 5000;
				buffState2 = 1;
				velocity2 = 200;
				buffTiming2 = 5;
			}
			cw.setVisible(false);
			c.setVisible(true);
			snow_fastArrayImage[i].setY(30);
			snow_fastArrayImage[i].setVisible(false);
			slow[playerNum - 1].setVisible(false);
			fast[playerNum - 1].setVisible(true);
			minus[playerNum - 1].setVisible(false);
			PlaySound(TEXT("sounds\\collect.wav"), NULL, SND_ASYNC);
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
			characterWalking.setVisible(true);
			characterWalking.update(frameTime);
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
			characterWalking.setVisible(true);
			characterWalking.update(frameTime);
		}
	}
	if (input->wasKeyPressed(VK_UP))             // if jump
	{
		jumping = true;					//to trigger jump
		input->clearKeyPress(VK_UP);
		stateOfDownJump = true;
	}
	if (!input->isKeyDown(VK_DOWN) && stateOfDownJump == true)
	{
		stateOfDownJump = false;
		PlaySound(TEXT("sounds\\jump.wav"), NULL, SND_ASYNC);
	}
	else
	{
		characterWalking.update(frameTime);
	}
}
void Hailo::characterControl2()
{ //Character player 2
	if (input->isKeyDown(0x44))     // if move right
	{

		if (character2.getX() > GAME_WIDTH - character2.getWidth())               // if off screen right
		{

		}
		else
		{
			character2.setVisible(false);
			character2.setX(character2.getX() + frameTime * velocity);
			characterWalking2.flipHorizontal(false);
			characterWalking2.setX(character2.getX() + frameTime * velocity);
			characterWalking2.setVisible(true);
			characterWalking2.update(frameTime);
		}

	}
	else{
		character2.setVisible(true);
		characterWalking2.setVisible(false);
	}
	if (input->isKeyDown(0x41))             // if move left
	{

		if (character2.getX() < 0)         // if off screen left
		{
		}
		else
		{
			character2.setVisible(false);
			character2.setX(character2.getX() - frameTime * velocity);
			characterWalking2.flipHorizontal(true);
			characterWalking2.setX(character2.getX() - frameTime * velocity);
			characterWalking2.setVisible(true);
			characterWalking2.update(frameTime);
		}
	}
	if (input->wasKeyPressed(0x57))             // if jump
	{
		jumping2 = true;					//to trigger jump
		input->clearKeyPress(0x57);
	}
	if (input->isKeyDown(0x57))             // if jump
	{
		jumping2 = true;					//to trigger jump
	}
	//character controls...
	characterWalking2.update(frameTime);
}


//when character is hit by hail, this method checks for the frozen state
void Hailo::frozen(Image c,Image cw, int playerNum)
{
	freeze[playerNum - 1].setY(c.getY() + c.getHeight() / 4);
	//to check if the freeze image x needs an update
	if (freeze[playerNum - 1].getX() != c.getX())
	{
		freeze[playerNum - 1].setX(c.getX());

	}

	if (playerNum == 1)
	{
		cw.setVisible(false);
		c.setVisible(true);
		freeze[playerNum - 1].setVisible(freezeState);
		if (freezeState && c.getY() == (GAME_HEIGHT - c.getHeight() - 160.0f))
		{

			freeze[playerNum - 1].setX(c.getX());
			unfreeze(playerNum, c);
		}
	}
	else
	{
		cw.setVisible(false);
		c.setVisible(true);
		freeze[playerNum - 1].setVisible(freezeState2);
		if (freezeState2 && c.getY() == (GAME_HEIGHT - c.getHeight() - 160.0f))
		{
			freeze[playerNum - 1].setX(c.getX());
			unfreeze(playerNum, c);
		}
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
				if (character.getY() != GAME_HEIGHT - character.getHeight() - 160.0f)
				{
					character.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
					characterWalking.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
				}
				jumping = false;
				increasingYAxisJump = true;
			}
		}
	}
}
void Hailo::jumpingMethod2()
{
	if (jumping2)
	{
		if (increasingYAxisJump2)
		{
			if (JumpTimer2 < 150)
			{
				JumpTimer2 += 3;
				//increase Y axis
				character2.setY(character2.getY() - frameTime * CHARACTER_JUMP_SPEED);
				characterWalking2.setY(characterWalking2.getY() - frameTime * CHARACTER_JUMP_SPEED);
			}
			else
			{
				if (character2.getY() <371 || character2.getY() >371 || characterWalking2.getY()<371 || characterWalking2.getY()>371)
				{
					character2.setY(371);
					characterWalking2.setY(371);
				}
				else
				{
					increasingYAxisJump2 = false;
				}
			}
		}
		else
		{
			if (JumpTimer2 > 0)
			{
				JumpTimer2 -= 3;
				character2.setY(character2.getY() + frameTime * CHARACTER_JUMP_SPEED);
				characterWalking2.setY(characterWalking2.getY() + frameTime * CHARACTER_JUMP_SPEED);
			}
			else
			{
				if (character2.getY() != GAME_HEIGHT - character2.getHeight() - 160.0f)
				{
					character2.setY(GAME_HEIGHT - character2.getHeight() - 160.0f);
					characterWalking2.setY(GAME_HEIGHT - character2.getHeight() - 160.0f);
				}
				jumping2 = false;
				increasingYAxisJump2 = true;
			}
		}
	}
}
/*Description:When the character is frozen, 
time limit and the number of times the character
press down is recorded. The number of times the down is pressed is to speed up the freezing state*/
void Hailo::unfreeze(int playerNum, Image c)
{
	if (playerNum == 1)
	{
	if (input->isKeyDown(VK_DOWN))
	{
		stateOfDown = true;		
		freeze[playerNum - 1].setX(c.getX() + 20);
	}
	if (!input->isKeyDown(VK_DOWN) && stateOfDown == true)
	{
		stateOfUp = true;		
		freeze[playerNum - 1].setX(c.getX() - 20);
	}
	if (stateOfUp)
	{
		stateOfDown = false;
		stateOfUp = false;
		countDownKey++;
		unfreezeTimer -= 0.01;				
	}
		if (countDownKey >= 20 || (gameTime - unfreezeTimer) >= FREEZETIME)//around 10 seconds or key was pressed and released 20 or more times
		{
			enableKey = true;
			countDownKey = 0;
			unfreezeTimer = gameTime;
			freezeState = false;
			freeze[playerNum - 1].setVisible(freezeState);
		}
	}
	else
	{
		if (input->isKeyDown(0x53))
		{
			stateOfDown2 = true;
			freeze[playerNum - 1].setX(c.getX() + 20);
		}
		if (!input->isKeyDown(0x53) && stateOfDown2 == true)
		{
			stateOfUp2 = true;
			freeze[playerNum - 1].setX(c.getX() - 20);
		}
		if (stateOfUp2)
		{
			stateOfDown2 = false;
			stateOfUp2 = false;
			countDownKey2++;
			unfreezeTimer2 -= 0.01;;
		}
		if (countDownKey2 >= 20 || (gameTime - unfreezeTimer2) >= FREEZETIME)//around 10 seconds or key was pressed and released 20 or more times
		{
			enableKey2 = true;
			countDownKey2 = 0;
			unfreezeTimer2 = gameTime;
			freezeState2 = false;
			freeze[playerNum - 1].setVisible(freezeState2);
		}
	}
}
void Hailo::importImage()
{
	// start page texture
	if (!startPageTexture.initialize(graphics, START_PAGE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing start page texture"));

	// start page
	if (!startPage.initialize(graphics, 0, 0, 0, &startPageTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	//character type Image
	//PLAYER 1
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

	//PLAYER 2
	// characterWalking texture
	if (!characterWalkingTexture2.initialize(graphics, CHARACTERWALKING2_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character walking texture"));

	// characterWalking
	if (!characterWalking2.initialize(graphics, CHARACTERWALKING_WIDTH, CHARACTERWALKING_HEIGHT, CHARACTERWALKING_COLS, &characterWalkingTexture2))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	// character texture
	if (!characterTexture2.initialize(graphics, CHARACTER2_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	// character
	if (!character2.initialize(graphics, CHARACTER_WIDTH, CHARACTER_HEIGHT, CHARACTER_COLS, &characterTexture2))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));


	//Objects dropping from the sky
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

	// minus texture
	if (!minusTexture.initialize(graphics, MINUS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing minus texture"));

	// slow texture
	if (!slowTexture.initialize(graphics, SLOW_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing slow texture"));


	// freeze
	if (!freeze[0].initialize(graphics, 0, 0, 0, &freezeTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// minus
	if (!minus[0].initialize(graphics, 0, 0, 0, &minusTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// slow
	if (!slow[0].initialize(graphics, 0, 0, 0, &slowTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	
	// freeze
	if (!freeze[1].initialize(graphics, 0, 0, 0, &freezeTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// minus
	if (!minus[1].initialize(graphics, 0, 0, 0, &minusTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// slow
	if (!slow[1].initialize(graphics, 0, 0, 0, &slowTexture))
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
	if (!fast[0].initialize(graphics, 0, 0, 0, &fastTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// fast
	if (!fast[1].initialize(graphics, 0, 0, 0, &fastTexture))
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
void Hailo::buffStateCheck(Image c,Image cw, int playerNum)
{
	if (playerNum == 1)
	{
		if (buffState == 1)//fast speed
		{
			fast[playerNum - 1].setX(c.getCenterX() - fast[playerNum - 1].getWidth() / 2);
			fast[playerNum - 1].setY(c.getCenterY() - fast[playerNum - 1].getHeight() / 2);
			if ((gameTime - buffForEffectTime) > BUFFTIME && buffTiming > 0)
			{
				buffTiming--;
				fast[playerNum - 1].update(frameTime);
				buffForEffectTime = gameTime;
			}
			if (buffTiming <= 0)
			{
				buffState = 0;
				velocity = 100;
				fast[playerNum - 1].setVisible(false);
				buffTiming = 5;
			}
		}
		else if (buffState == 2)
		{
			//animation
			if (invincibleTime <= 2)
			{
				c.setVisible(false);
				cw.setVisible(false);
			}
			else
			{
				if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
					cw.setVisible(true);
				else
					c.setVisible(true);
			}
			if (invincibleTime >= 4)
				invincibleTime = 0;
			invincibleTime++;
			if ((gameTime - buffForEffectTime) > BUFFTIME&&buffTiming > 0)
			{
				buffTiming--;
				buffForEffectTime = gameTime;
			}
			if (buffTiming <= 0)
			{
				if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
				{
					cw.setVisible(true);
				}
				else
				{
					c.setVisible(true);
				}
				buffState = 0;
				invincibleTime = 1000;
				buffTiming = 5;
			}
		}
		else if (buffState == 3)//slow speed
		{
			slow[playerNum - 1].setX(c.getCenterX() - slow[playerNum - 1].getWidth() / 2);
			slow[playerNum - 1].setY(c.getCenterY() - slow[playerNum - 1].getHeight() / 2);
			if ((gameTime - buffForEffectTime) > BUFFTIME&&buffTiming > 0)
			{
				buffTiming--;
				slow[playerNum - 1].setVisible(true);
				buffForEffectTime = gameTime;
				slow[playerNum - 1].update(frameTime);
			}
			if (buffTiming <= 0)
			{
				slow[playerNum - 1].setVisible(false);
				buffState = 0;
				velocity = 100;
				buffTiming = 5;
			}
		}
		else if (buffState == 4)//minus points
		{
			minus[playerNum - 1].setX(c.getCenterX() - minus[playerNum - 1].getWidth() / 2);
			minus[playerNum - 1].setY(c.getCenterY() - minus[playerNum - 1].getHeight() / 2);
			if ((gameTime - buffForEffectTime) > BUFFTIME&&buffTiming > 0)
			{
				buffTiming--;
				minus[playerNum - 1].setVisible(true);
				buffForEffectTime = gameTime;
				minus[playerNum - 1].update(frameTime);
			}
			if (buffTiming <= 0)
			{
				minus[playerNum - 1].setVisible(false);
				buffState = 0;
				buffTiming = 5;
			}
		}
	}
	else
	{
		if (buffState2 == 1)//fast speed
		{
			fast[playerNum - 1].setX(c.getCenterX() - fast[playerNum - 1].getWidth() / 2);
			fast[playerNum - 1].setY(c.getCenterY() - fast[playerNum - 1].getHeight() / 2);
			if ((gameTime - buffForEffectTime2) > BUFFTIME && buffTiming2 > 0)
			{
				buffTiming2--;
				fast[playerNum - 1].update(frameTime);
				buffForEffectTime2 = gameTime;
			}
			if (buffTiming2 <= 0)
			{
				buffState2 = 0;
				velocity2 = 100;
				fast[playerNum - 1].setVisible(false);
				buffTiming2 = 5;
			}
		}
		else if (buffState2 == 2)
		{
			//animation
			if (invincibleTime2 <= 2)
			{
				c.setVisible(false);
				cw.setVisible(false);
			}
			else
			{
				if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
					cw.setVisible(true);
				else
					c.setVisible(true);
			}
			if (invincibleTime2 >= 4)
				invincibleTime2 = 0;
			invincibleTime2++;
			if ((gameTime - buffForEffectTime2) > BUFFTIME&&buffTiming2 > 0)
			{
				buffTiming2--;
				buffForEffectTime2 = gameTime;
			}
			if (buffTiming2 <= 0)
			{
				if (input->isKeyDown(0x44) || input->isKeyDown(0x53) || input->isKeyDown(0x57))
				{
					cw.setVisible(true);
				}
				else
				{
					c.setVisible(true);
				}
				buffState2 = 0;
				invincibleTime2 = 1000;
				buffTiming2 = 5;
			}
		}
		else if (buffState2 == 3)//slow speed
		{
			slow[playerNum - 1].setX(c.getCenterX() - slow[playerNum - 1].getWidth() / 2);
			slow[playerNum - 1].setY(c.getCenterY() - slow[playerNum - 1].getHeight() / 2);
			if ((gameTime - buffForEffectTime2) > BUFFTIME&&buffTiming > 0)
			{
				buffTiming2--;
				slow[playerNum - 1].setVisible(true);
				buffForEffectTime2= gameTime;
				slow[playerNum - 1].update(frameTime);
			}
		if (buffTiming2 <= 0)
			{
				slow[playerNum - 1].setVisible(false);
				buffState2 = 0;
				velocity2 = 100;
				buffTiming2 = 5;
			}
		}
		else if (buffState2 == 4)//minus points
		{
			minus[playerNum - 1].setX(c.getCenterX() - minus[playerNum - 1].getWidth() / 2);
			minus[playerNum - 1].setY(c.getCenterY() - minus[playerNum - 1].getHeight() / 2);
			if ((gameTime - buffForEffectTime2) > BUFFTIME&&buffTiming2 > 0)
			{
				buffTiming2--;
				minus[playerNum - 1].setVisible(true);
				buffForEffectTime2 = gameTime;
				minus[playerNum - 1].update(frameTime);
			}
			if (buffTiming2 <= 0)
			{
				minus[playerNum - 1].setVisible(false);
				buffState2 = 0;
				buffTiming2 = 5;
			}
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
		gameStart = 3;
		paused = true;
		return 0;
	}
}

