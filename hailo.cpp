
#include "hailo.h"
#include <iostream>
using namespace std;

//=============================================================================
// Constructor
//=============================================================================
Hailo::Hailo()
{}

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
	snowman.setX(GAME_WIDTH/2);
	snowman.setY(GAME_HEIGHT - snowman.getHeight() - 160.0f);
	snowman.setCurrentFrame(SNOWMAN_START_FRAME);
	snowman.setFrames(SNOWMAN_START_FRAME, SNOWMAN_END_FRAME);
	snowman.setFrameDelay(SNOWMAN_ANIMATION_DELAY);
	freeze.setVisible(false);

	//characters
	character.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
	characterWalking.setY(GAME_HEIGHT - character.getHeight() - 160.0f);
	characterWalking.setCurrentFrame(CHARACTERWALKING_START_FRAME);
	characterWalking.setFrames(CHARACTERWALKING_START_FRAME,CHARACTERWALKING_END_FRAME);
	characterWalking.setFrameDelay(CHARACTERWALKING_ANIMATION_DELAY);
	characterWalking.setVisible(false);
	character.setVisible(true);
	
	
	
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
		unfreeze();
	}
	characterWalking.update(frameTime);
	snowman.update(frameTime);
	frozen();
	
	jumpingMethod();
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
	graphics->spriteEnd();                  // end drawing sprites

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
    Game::resetAll();
    return;
}
//spawning hail and snow
void Hailo::itemSpawn()
{
	gameTime += frameTime;//using time from game class
	
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snowArrayImage[i].setDegrees(snowArrayImage[i].getDegrees() + 5);// snow rotating
		if (snowArrayImage[i].getVisible())
		{

			snowArrayImage[i].setY(snowArrayImage[i].getY() + frameTime * CHARACTER_SPEED);//snow travelling downwards

			if (snowArrayImage[i].getY() > 600)// checking for item out of screen
			{
				snowArrayImage[i].setY(30);//reset snow position
				snowArrayImage[i].setVisible(false);//reuse snow object.
			}

		}
		else
		{
			if ((gameTime - lastSnowSpawnTime) > (rand() % 3 + 5))
			{
				snowArrayImage[i].setX(getNonDuplicateRanNum() * (GAME_WIDTH / 20) + snowArrayImage[i].getWidth() / 2);
				snowArrayImage[i].setVisible(true);//spawn snow
				lastSnowSpawnTime = gameTime;//reset spawn time(r)
				cout << "Snow: " << snowArrayImage[i].getX() << " , " << snowArrayImage[i].getY() << endl;
				cout << "GameTime: " << gameTime << endl;
			}
		}
	}
	for (int i = 0; i < (sizeof(hailArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		hailArrayImage[i].setDegrees(hailArrayImage[i].getDegrees() + 5);// hail rotating
		if (hailArrayImage[i].getVisible())
		{

			hailArrayImage[i].setY(hailArrayImage[i].getY() + frameTime * CHARACTER_SPEED);//hail travelling downwards

			if (hailArrayImage[i].getY() > GAME_HEIGHT)// checking for item out of screen
			{
				hailArrayImage[i].setY(30);//reset hail position
				hailArrayImage[i].setVisible(false);//reuse hail object.
			}

		}
		else
		{
			if ((gameTime - lastHailSpawnTime) > (rand() % 3 + 5))
			{
				hailArrayImage[i].setX(getNonDuplicateRanNum() * (GAME_WIDTH / 20) + hailArrayImage[i].getWidth() / 2);
				hailArrayImage[i].setVisible(true);//spawn hail
				lastHailSpawnTime = gameTime;//reset spawn time(r)
				cout << "Hail: " << hailArrayImage[i].getX() << " , " << hailArrayImage[i].getY() << endl;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_fastArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_fastArrayImage[i].setDegrees(snow_fastArrayImage[i].getDegrees() + 5);// snow_fast rotating
		if (snow_fastArrayImage[i].getVisible())
		{

			snow_fastArrayImage[i].setY(snow_fastArrayImage[i].getY() + frameTime * CHARACTER_SPEED);//snow_fast travelling downwards

			if (snow_fastArrayImage[i].getY() > GAME_HEIGHT)// checking for item out of screen
			{
				snow_fastArrayImage[i].setY(30);//reset snow_fast position
				snow_fastArrayImage[i].setVisible(false);//reuse snow_fast object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_fastSpawnTime) > (rand() % 3 + 5))
			{
				snow_fastArrayImage[i].setX(getNonDuplicateRanNum() * (GAME_WIDTH / 20) + snow_fastArrayImage[i].getWidth() / 2);
				snow_fastArrayImage[i].setVisible(true);//spawn snow_fast
				lastsnow_fastSpawnTime = gameTime;//reset spawn time(r)
				cout << "snow_fast: " << snow_fastArrayImage[i].getX() << " , " << snow_fastArrayImage[i].getY() << endl;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_invincibleArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_invincibleArrayImage[i].setDegrees(snow_invincibleArrayImage[i].getDegrees() + 5);// snow_invincible rotating
		if (snow_invincibleArrayImage[i].getVisible())
		{

			snow_invincibleArrayImage[i].setY(snow_invincibleArrayImage[i].getY() + frameTime * CHARACTER_SPEED);//snow_invincible travelling downwards

			if (snow_invincibleArrayImage[i].getY() > GAME_HEIGHT)// checking for item out of screen
			{
				snow_invincibleArrayImage[i].setY(30);//reset snow_invincible position
				snow_invincibleArrayImage[i].setVisible(false);//reuse snow_invincible object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_invincibleSpawnTime) > (rand() % 3 + 5))
			{
				snow_invincibleArrayImage[i].setX(getNonDuplicateRanNum() * (GAME_WIDTH / 20) + snow_invincibleArrayImage[i].getWidth() / 2);
				snow_invincibleArrayImage[i].setVisible(true);//spawn snow_invincible
				lastsnow_invincibleSpawnTime = gameTime;//reset spawn time(r)
				cout << "snow_invincible: " << snow_invincibleArrayImage[i].getX() << " , " << snow_invincibleArrayImage[i].getY() << endl;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_minusArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_minusArrayImage[i].setDegrees(snow_minusArrayImage[i].getDegrees() + 5);// snow_minus rotating
		if (snow_minusArrayImage[i].getVisible())
		{

			snow_minusArrayImage[i].setY(snow_minusArrayImage[i].getY() + frameTime * CHARACTER_SPEED);//snow_minus travelling downwards

			if (snow_minusArrayImage[i].getY() > GAME_HEIGHT)// checking for item out of screen
			{
				snow_minusArrayImage[i].setY(30);//reset snow_minus position
				snow_minusArrayImage[i].setVisible(false);//reuse snow_minus object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_minusSpawnTime) > (rand() % 3 + 5))
			{
				snow_minusArrayImage[i].setX(getNonDuplicateRanNum() * (GAME_WIDTH / 20) + snow_minusArrayImage[i].getWidth() / 2);
				snow_minusArrayImage[i].setVisible(true);//spawn snow_minus
				lastsnow_minusSpawnTime = gameTime;//reset spawn time(r)
				cout << "snow_minus: " << snow_minusArrayImage[i].getX() << " , " << snow_minusArrayImage[i].getY() << endl;
			}
		}
	}
	for (int i = 0; i < (sizeof(snow_slowArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snow_slowArrayImage[i].setDegrees(snow_slowArrayImage[i].getDegrees() + 5);// snow_slow rotating
		if (snow_slowArrayImage[i].getVisible())
		{

			snow_slowArrayImage[i].setY(snow_slowArrayImage[i].getY() + frameTime * CHARACTER_SPEED);//snow_slow travelling downwards

			if (snow_slowArrayImage[i].getY() > GAME_HEIGHT)// checking for item out of screen
			{
				snow_slowArrayImage[i].setY(30);//reset snow_slow position
				snow_slowArrayImage[i].setVisible(false);//reuse snow_slow object.
			}

		}
		else
		{
			if ((gameTime - lastsnow_slowSpawnTime) > (rand() % 3 + 5))
			{
				snow_slowArrayImage[i].setX(getNonDuplicateRanNum() * (GAME_WIDTH / 20) + snow_slowArrayImage[i].getWidth() / 2);
				snow_slowArrayImage[i].setVisible(true);//spawn snow_slow
				lastsnow_slowSpawnTime = gameTime;//reset spawn time(r)
				cout << "snow_slow: " << snow_slowArrayImage[i].getX() << " , " << snow_slowArrayImage[i].getY() << endl;
			}
		}
	}

	for (int i = 0; i < (sizeof(preventSameColumnSpawning) / sizeof(int)); i++)
	{
		preventSameColumnSpawning[i] = 0;
	}
}
boolean Hailo::collisionDetection(){
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++){
		// character and snow
		if ((character.getX() + character.getWidth() - 20) >= (snowArrayImage[i].getX()) &&
			(character.getX() + 15) <= (snowArrayImage[i].getX() + snowArrayImage[i].getWidth()) &&
			(character.getY() + character.getHeight()) >= (snowArrayImage[i].getY()) &&
			(character.getY() + 10) <= (snowArrayImage[i].getY() + snowArrayImage[i].getHeight())){

			snowArrayImage[i].setY(30);//reset snow position
			snowArrayImage[i].setVisible(false);//reuse snow object.
			return true;
		}
		// character walking and snow
		if ((characterWalking.getX() + characterWalking.getWidth() - 20) >= (snowArrayImage[i].getX()) &&
			(characterWalking.getX() + 15) <= (snowArrayImage[i].getX() + snowArrayImage[i].getWidth()) &&
			(characterWalking.getY() + characterWalking.getHeight()) >= (snowArrayImage[i].getY()) &&
			(characterWalking.getY() + 10) <= (snowArrayImage[i].getY() + snowArrayImage[i].getHeight())){

			snowArrayImage[i].setY(30);//reset snow position
			snowArrayImage[i].setVisible(false);//reuse snow object.
			return true;
		}
		// character and hail
		if ((character.getX() + character.getWidth() - 20) >= (hailArrayImage[i].getX()) &&
			(character.getX() + 15) <= (hailArrayImage[i].getX() + hailArrayImage[i].getWidth()) &&
			(character.getY() + character.getHeight()) >= (hailArrayImage[i].getY()) &&
			(character.getY() + 10) <= (hailArrayImage[i].getY() + hailArrayImage[i].getHeight())){

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
			enableKey = false;
			characterWalking.setVisible(false);
			character.setVisible(true);
			freezeState = true;
			hailArrayImage[i].setY(30);//reset hail position
			hailArrayImage[i].setVisible(false);//reuse hail object.
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

//character moving left and right and up to jump
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
			character.setX(character.getX() + frameTime * CHARACTER_SPEED);
			characterWalking.flipHorizontal(false);
			characterWalking.setX(character.getX() + frameTime * CHARACTER_SPEED);
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
			character.setX(character.getX() - frameTime * CHARACTER_SPEED);
			characterWalking.flipHorizontal(true);
			characterWalking.setX(character.getX() - frameTime * CHARACTER_SPEED);
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
			cout << "Character: " << characterWalking.getX() << endl;
		}
	}
	if (input->wasKeyPressed(VK_UP))             // if jump
	{
		jumping = true;					//to trigger jump
		cout << "true" << endl;
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
	freeze.setX(character.getX());
	freeze.setY(character.getY()+character.getHeight()/4);
	freeze.setVisible(freezeState);
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
				//cout << character.getY() << endl;
			}
			else
				increasingYAxisJump = false;
		}
		else
		{
			if (JumpTimer >0)
			{
				JumpTimer -= 3;
				character.setY(character.getY() + frameTime * CHARACTER_JUMP_SPEED);
				characterWalking.setY(characterWalking.getY() + frameTime * CHARACTER_JUMP_SPEED);
			}
			else
			{
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
	}
	if (!input->isKeyDown(VK_DOWN) && stateOfDown == true)
	{
		stateOfUp = true;
	}

	if (stateOfDown == true && stateOfUp == true)
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
	// snowman texture
	if (!snowmanTexture.initialize(graphics, SNOWMAN_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman texture"));

	// snow_minus texture
	if (!snow_minusTexture.initialize(graphics, SNOW_MINUS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_minus texture"));

	// snow_minus
	if (!snow_minus.initialize(graphics, 0, 0, 0, &snow_minusTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
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
		cout << "Ran: " << tempRan << endl;
		for (int i = 0; i < (sizeof(preventSameColumnSpawning) / sizeof(int)) - 1; i++)
		{
			cout << "RanArray: " << preventSameColumnSpawning[i] << endl;
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
					return tempRan;
				}
			}
			else
			{
				preventSameColumnSpawning[i] = tempRan;
				return tempRan;
			}
		}
	} while (repeat);
	
	
}