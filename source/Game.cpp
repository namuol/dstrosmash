/*
 * Game - a Game object is an instance of an ASTROSMASH! game.
 */
#include "Game.h"
#include "bg.h" // The background image

#include <list>
using namespace std;

UL_IMAGE * Game::bgImg = NULL;

Game::Game() {
    bgImg = ulLoadImageFilePNG((const char*)bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL2);
    theMan = new Man(this, 128, FLOOR);	
}

Game::~Game() {
    delete theMan;
    delete bgImg;
}

void Game::update() {
	//Read keys
	ulReadKeys(0);

    theMan->update();

    // Update each shot.
    //  We need to use a temporary list to prevent invalidating the 
    //  iterator when deleting a shot that goes off the screen or collides with something.
    list<Shot *>::iterator s;
    list<Shot *> tmpShots( shots ); 
    for(s = tmpShots.begin(); s != tmpShots.end(); ++s ) {
        (*s)->update();
    }

    // Update each rock.
    list<Rock *>::iterator r;
    list<Rock *> tmpRocks( rocks ); 
    for(r = tmpRocks.begin(); r != tmpRocks.end(); ++r ) {
        (*r)->update();
    }
}

void Game::draw() {
    ulStartDrawing2D();

    // First draw background
    ulDrawImageXY( bgImg, 0, 0 );

    // Then draw the man
    theMan->draw();

    // Draw all the shots:
    std::list<Shot *>::iterator s;
    for(s = shots.begin(); s != shots.end(); ++s ) {
        (*s)->draw();
    }
    
    // Draw all the rocks:
    std::list<Rock *>::iterator r;
    for(r = rocks.begin(); r != rocks.end(); ++r ) {
        (*r)->draw();
    }

	//End the drawing
	ulEndDrawing();
		
	//Wait the VBlank (synchronize at 60 fps)
	ulSyncFrame();
}

void Game::mainLoop() {
	while(1) {
        if(RAND(30)==0)
            rocks.push_back(new Rock(this, 0,0));
        update();
        draw();
	}
}
