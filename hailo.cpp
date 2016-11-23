
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
    Game::initialize(hwnd); // throws GameError
	

		// character texture
	if (!characterTexture.initialize(graphics, CHARACTER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	// character
	if (!character.initialize(graphics, 0, 0, 0, &characterTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	// background texture
	if (!backgroundTexture.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// background
	if (!background.initialize(graphics, 0, 0, 0, &backgroundTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	// cloud texture
	if (!cloudTexture.initialize(graphics, CLOUD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// cloud
	if (!cloud.initialize(graphics, 0, 0, 0, &cloudTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	
	character.setY(GAME_HEIGHT - character.getHeight() - 123);
	
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Hailo::update()
{
	if (cloud.getX() > GAME_WIDTH) // position off screen left
	{
		cloud.setX((float)-cloud.getWidth());
	}
	else
	{
		cloud.setX(cloud.getX() + frameTime * CHARACTER_SPEED);
	}
	character.update(frameTime);
	if (character.getX() > GAME_WIDTH)               // if off screen right
	{
		character.setX((float)-character.getWidth());     // position off screen left
	}
	character.update(frameTime);
	if (input->isKeyDown(VK_RIGHT))            // if move right
	{
		
		if (character.getX() > GAME_WIDTH - character.getWidth())               // if off screen right
		{		
			
		}
		else
		{
			character.setX(character.getX() + frameTime * CHARACTER_SPEED);
		}
			
	}
	if (input->isKeyDown(VK_LEFT))             // if move left
	{
		
		if (character.getX() < 0)         // if off screen left
		{
	
		}
		else
		{
			character.setX(character.getX() - frameTime * CHARACTER_SPEED);
		}
	}
	character.update(frameTime);
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
	cloud.draw();
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
    Game::resetAll();
    return;
}
