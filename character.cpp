#include "character.h"

Character::Character() : Entity()
{
	spriteData.width = CHARACTER_WIDTH;
	spriteData.height = CHARACTER_HEIGHT;
	frameDelay = CHARACTER_ANIMATION_DELAY;
	startFrame = CHARACTER_START_FRAME;     // first frame of ship animation
	endFrame = CHARACTER_END_FRAME;     // last frame of ship animation
	currentFrame = startFrame;
	collisionType = entityNS::BOX;
}
void Character::draw()
{
	Image::draw();
	character.draw();
}
void Character::update(float frameTime)
{
	Entity::update(frameTime);

}
bool Character::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
	if (!characterWalkingTexture.initialize(gamePtr->getGraphics(), CHARACTERWALKING2_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character walking texture"));

	// characterWalking
	if (!characterWalking.initialize(gamePtr->getGraphics(), CHARACTERWALKING_WIDTH, CHARACTERWALKING_HEIGHT, CHARACTERWALKING_COLS, &characterWalkingTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));
	return (Entity::initialize(gamePtr, width, height, ncols, textureM));
}