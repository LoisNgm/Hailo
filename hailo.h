
#ifndef _HAILO_H             // prevent multiple definitions if this 
#define _HAILO_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

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
	TextureManager cloudTexture;
	Image cloud;

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
};

#endif
