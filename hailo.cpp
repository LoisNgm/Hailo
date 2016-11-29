
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
	characterWalking.update(frameTime);
	Game::initialize(hwnd); // throws GameError
	
	importImage();
	
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
	character.draw();
	characterWalking.draw();
	snow.draw();
	hail.draw();
	cloud.draw();
	freeze.draw();
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++)
	{
		snowArrayImage[i].draw();
	}
	for (int i = 0; i < (sizeof(hailArrayImage) / sizeof(Image)); i++)
	{
		hailArrayImage[i].draw();
	}
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
    Game::resetAll();
    return;
}
//spawning hail and snow
void Hailo::itemSpawn()
{
	gameTime += frameTime;//using time from game class
	srand(time(0));
	for (int i = 0; i < (sizeof(snowArrayImage) / sizeof(Image)); i++)// looping through the array
	{
		snowArrayImage[i].setDegrees(snowArrayImage[i].getDegrees() + 5);// snow rotating
		if (snowArrayImage[i].getVisible())
		{
			
				snowArrayImage[i].setY(snowArrayImage[i].getY() + frameTime * CHARACTER_SPEED);//snow travelling downwards
				
				if (snowArrayImage[i].getY() > GAME_HEIGHT)// checking for item out of screen
				{
					snowArrayImage[i].setY(30);//reset snow position
					snowArrayImage[i].setVisible(false);//reuse snow object.
				}
			
		}
		else
		{
			if ((gameTime - lastSnowSpawnTime) > 2)
			{
				snowArrayImage[i].setX((rand()%20 +1) * (GAME_WIDTH /20) + snowArrayImage[i].getWidth()/2);
				snowArrayImage[i].setVisible(true);//spawn snow
				lastSnowSpawnTime = gameTime;//reset spawn time(r)
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
			if ((gameTime - lastHailSpawnTime) > 2)
			{
				hailArrayImage[i].setX((rand() % 20 + 1) * (GAME_WIDTH / 20) + hailArrayImage[i].getWidth() / 2);
				hailArrayImage[i].setVisible(true);//spawn hail
				lastHailSpawnTime = gameTime;//reset spawn time(r)
			}
		}
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
// cloud moving
void Hailo::cloudAnimation()
{
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
		}
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
	// snow_fast texture
	if (!snow_fastTexture.initialize(graphics, SNOW_FAST_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snow_fast texture"));

	// snow_fast
	if (!snow_fast.initialize(graphics, 0, 0, 0, &snow_fastTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// snow_invinicible texture
	if (!snow_invicibleTexture.initialize(graphics, SNOW_INVINCIBLE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snow_invinicible texture"));

	// snow_invinicible
	if (!snow_invicible.initialize(graphics, 0, 0, 0, &snow_invicibleTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// snowman texture
	if (!snowmanTexture.initialize(graphics, SNOWMAN_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman texture"));

	// snowman
	if (!snowman.initialize(graphics, 0, 0, 0, &snowmanTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// snowman_head texture
	if (!snowman_headTexture.initialize(graphics, SNOWMAN_HEAD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_head texture"));

	// snowman_head
	if (!snowman_head.initialize(graphics, 0, 0, 0, &snowman_headTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// snow_minus texture
	if (!snow_minusTexture.initialize(graphics, SNOW_MINUS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_minus texture"));

	// snow_minus
	if (!snow_minus.initialize(graphics, 0, 0, 0, &snow_minusTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// snowman_head texture
	if (!snowman_headTexture.initialize(graphics, SNOWMAN_HEAD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing snowman_minus texture"));

	// snowman_head
	if (!snowman_head.initialize(graphics, 0, 0, 0, &snowman_headTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
}