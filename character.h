#ifndef _CHARACTER_H             // prevent multiple definitions if this 
#define _CHARACTER_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include "entity.h"
#include "constants.h"
#include "textureManager.h"

class Character : public Entity
{
private: 
	Image character;
	Image characterWalking;
	TextureManager characterWalkingTexture;
	int score;
	int health;

public:
	Character();
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);
	void update(float frameTime);
	void drawCharacter();

	Image getCharacter() { return character; };
	Image getCharacterWalking() { return characterWalking; };
	void setCharacter(Image c) { character = c; };
	void setCharacterWalking(Image c) { characterWalking = c; };

};
#endif