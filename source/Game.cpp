/*
 * Game - a Game object is an instance of an ASTROSMASH! game.
 */
#include "Game.h"
#include "Man.h"
#include "main.h"
#include "bg.h" // The background image

#include <list>
using namespace std;

UL_IMAGE * Game::bgImg = NULL;

const int Game::INTV_PALETTE[] = {
    RGB15(31,31,31), // White
    RGB15(31,28,10), // 
    RGB15(0,20,10),  // 
    RGB15(10,13,0),  // 
    RGB15(24,25,21), // 
    RGB15(31,7,2),   // Red 
    RGB15(0,5,31),   // Blue 
    RGB15(0,0,0)     // Black
};

Game::Game() {
    score = 0;
    peak_score = 0;
    multiplyer = 1;
    bgImg = ulLoadImageFilePNG((const char*)bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL2);
    theMan = new Man(this, 32, FLOOR-MAN_HEIGHT);	
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

    // Update each explosion.
    list<Explosion *>::iterator e;
    list<Explosion *> tmpExplosions( explosions ); 
    for(e = tmpExplosions.begin(); e != tmpExplosions.end(); ++e ) {
        (*e)->update();
    }

}

void Game::updateScore(int amount) {
    score += amount * multiplyer;
    if( score > peak_score )
        peak_score = score;
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

    list<Explosion *>::iterator e;
    for(e = explosions.begin(); e != explosions.end(); ++e ) {
        (*e)->draw();
    }


	int cpuTime = (TIMER1_DATA * 1000) / totalTime;
	ulPrintf_xy(0, 0, "CPU: %02i.%i%%", cpuTime / 10, cpuTime % 10);
	ulPrintf_xy(0, FLOOR*2, "%i          %ix", score, multiplyer);

	//End the drawing
	ulEndDrawing();


    ulDrawLine(LEFT_WALL*2, 0, LEFT_WALL*2, CEILING*2, RGB15(31,0,0));
    ulDrawLine(RIGHT_WALL*2, 0, RIGHT_WALL*2, CEILING*2, RGB15(31,0,0));
		
	//Wait the VBlank (synchronize at 60 fps)
	ulSyncFrame();
}

void Game::mainLoop() {

    //To avoid a divide by zero the first time
	totalTime = 1;


	while(1) {
        //Initialize the timers to measure the framerate
	    TIMER1_CR = 0;
	    TIMER1_DATA = 0;
	    TIMER1_CR = TIMER_DIV_64 | TIMER_ENABLE;

        if(RAND(30)==0)
            rocks.push_back(new Rock(this, (Rock *)0, 0, RAND(NUM_ROCK_IMAGES) ));
        update();
        draw();
		totalTime = TIMER1_DATA;
	}
}
