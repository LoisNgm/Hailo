
#include "hailo.h"

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

	character.setY(GAME_HEIGHT - character.getHeight() - 123.0f);

	characterWalking.setY(GAME_HEIGHT - character.getHeight() - 123.0f);
	characterWalking.setCurrentFrame(CHARACTERWALKING_START_FRAME);
	characterWalking.setFrames(CHARACTERWALKING_START_FRAME,CHARACTERWALKING_END_FRAME);
	characterWalking.setFrameDelay(CHARACTERWALKING_ANIMATION_DELAY);
	characterWalking.setVisible(false);
	character.setVisible(true);
	
	
	
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Hailo::update()
{
	itemSpawn();
	//cloud's animation....
	if (cloud.getX() > GAME_WIDTH) // position off screen left
	{
		cloud.setX((float)-cloud.getWidth());
	}
	else
	{
		cloud.setX(cloud.getX() + frameTime * CHARACTER_SPEED);
	}
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
			characterWalking.setVisible(true);
			characterWalking.update(frameTime);
		}
		
	}
	else
	{
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
			characterWalking.setVisible(true);
			characterWalking.update(frameTime);
		}
	}
	else
	{
		
	}
	characterWalking.update(frameTime);
	
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Hailo::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Hailo::collisions()
{}

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
    Game::resetAll();
    return;
}
void Hailo::itemSpawn()
{
	gameTime += frameTime;//using time from game class

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
				snowArrayImage[i].setX(((rand() % 20 + 1)*30) + 10);
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
				hailArrayImage[i].setX(((rand() % 20 + 1) * 30) + 10);
				hailArrayImage[i].setVisible(true);//spawn hail
				lastHailSpawnTime = gameTime;//reset spawn time(r)
			}
		}
	}
	
	

}


