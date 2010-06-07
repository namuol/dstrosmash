/*
 * Game - a Game object is an instance of an ASTROSMASH! game.
 */
#include "Game.h"
#include "Man.h"
#include "main.h"
#include "bg.h" // The background image

#include <list>
using namespace std;

// The background color for each level
const int Game::LEVEL_COLORS[] = {
    RED,   // Game Over
    BLACK, // x1
    BLUE,  // x2
    PURPLE,// x3
    CYAN,  // x4
    GRAY,  // x5
    BLACK  // x6
};

UL_IMAGE * Game::bgImg = NULL;

Game::Game() {
    one_up_total = 0;
    score = 0;
    peak_score = 0;
    multiplyer = 1;
    shake_amt = 0.0;
    lives = STARTING_LIVES; 
    bgImg = ulLoadImageFilePNG((const char*)bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL2);
    theMan = new Man(this, 32, FLOOR-MAN_HEIGHT);	

    shots = new list<Shot *>;
    rocks = new list<Rock *>;
    explosions = new list<Explosion *>;
    spinners = new list<Spinner *>;

    paused = false;
}

Game::~Game() {
    delete theMan;
    delete bgImg;
    delete shots;
    delete rocks;
    delete explosions;
    delete spinners;
}

void Game::update() {
	//Read keys
	ulReadKeys(0);

    if(ul_keys.pressed.start)
        paused = !paused;

    if(paused)
        return;

    theMan->update();

    //  We need to use a temporary list to prevent invalidating the 
    //  iterator when deleting a shot that goes off the screen or collides with something.
    // Update each shot.
    list<Shot *>::iterator s;
    list<Shot *> tmpShots( *shots ); 
    for(s = tmpShots.begin(); s != tmpShots.end(); ++s ) {
        (*s)->update();
    }

    // Update each rock.
    list<Rock *>::iterator r;
    list<Rock *> tmpRocks( *rocks ); 
    for(r = tmpRocks.begin(); r != tmpRocks.end(); ++r ) {
        (*r)->update();
    }

    // Update each spinner.
    list<Spinner *>::iterator sp;
    list<Spinner *> tmpSpinners( *spinners ); 
    for(sp = tmpSpinners.begin(); sp != tmpSpinners.end(); ++sp ) {
        (*sp)->update();
    }

    // Update each explosion.
    list<Explosion *>::iterator e;
    list<Explosion *> tmpExplosions( *explosions ); 
    for(e = tmpExplosions.begin(); e != tmpExplosions.end(); ++e ) {
        (*e)->update();
    }

    if(score >= X6_LEVEL_SCORE ) {
        multiplyer = 6;
    } else if( score >= X5_LEVEL_SCORE ) {
        multiplyer = 5;
    } else if( score >= X4_LEVEL_SCORE ) {
        multiplyer = 4;
    } else if( score >= X3_LEVEL_SCORE ) {
        multiplyer = 3;
    } else if( score >= X2_LEVEL_SCORE ) {
        multiplyer = 2;
    } else {
        multiplyer = 1;
    }
    if( lives < 0 ) {
        multiplyer = 0;
    }
}

void Game::updateScore(int amount) {
    score += amount * multiplyer;
    if( score > peak_score )
        peak_score = score;
    if( score > 0 ) {
        one_up_total += amount*multiplyer;
        if( one_up_total >= ONE_UP_SCORE ) {
            one_up_total = one_up_total - ONE_UP_SCORE;
            ++lives;
        }
    }
}

void Game::draw() {
    ulStartDrawing2D();

    // First draw background
    ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, LEVEL_COLORS[multiplyer]);

    int cpuTime = (TIMER1_DATA * 1000) / totalTime;
    ulPrintf_xy(0, 0, "CPU: %02i.%i%% %f %f", cpuTime / 10, cpuTime % 10, SHAKE_MAX, shake_amt);

    if( lives >= 0 ) {
        if( shake_amt > 0.0 ) {
            SHAKE(MIN((int)shake_amt,(int)SHAKE_MAX));
            shake_amt -= SHAKE_DECREASE;
        } else {
            shake_amt = 0.0;
        }
        ulDrawImageXY( bgImg, 0, 0 );

        // Then draw the man
        theMan->draw();

        // Draw all the shots:
        std::list<Shot *>::iterator s;
        for(s = shots->begin(); s != shots->end(); ++s ) {
            (*s)->draw();
        }
        
        // Draw all the rocks:
        std::list<Rock *>::iterator r;
        for(r = rocks->begin(); r != rocks->end(); ++r ) {
            (*r)->draw();
        }

        // Draw all the spinners:
        std::list<Spinner *>::iterator sp;
        for(sp = spinners->begin(); sp != spinners->end(); ++sp ) {
            (*sp)->draw();
        }

        // Draw all the explosions:
        list<Explosion *>::iterator e;
        for(e = explosions->begin(); e != explosions->end(); ++e ) {
            (*e)->draw();
        }


        ulPrintf_xy(0, FLOOR*2+7, "%i          %ix | Lives: %i", score, multiplyer, lives);
    } else {
        ulDrawImageXY( bgImg, 0, 0 );
        ulPrintf_xy(RIGHT_WALL-30, FLOOR, "GAME OVER");
        ulPrintf_xy(RIGHT_WALL-30, FLOOR+10, "Final Score: %i", score);
        ulPrintf_xy(RIGHT_WALL-30, FLOOR+20, " Peak Score: %i", peak_score);
    }
	//End the drawing
	ulEndDrawing();

    //ulDrawLine(LEFT_WALL*2, 0, LEFT_WALL*2, CEILING*2, RGB15(31,0,0));
    //ulDrawLine(RIGHT_WALL*2, 0, RIGHT_WALL*2, CEILING*2, RGB15(31,0,0));
		
	//Wait the VBlank (synchronize at 60 fps)
	ulSyncFrame();
    ulResetScreenView();
}

void Game::death() {
    // This gets called each time the player dies. It should be called within
    // the update() call in mainLoop.
    //  It enters it's own loop to display the death animation and 
    //  then returns to the mainLoop in a natural fashion.
    --lives;
    list<Shot *> *old_shots = shots;
    list<Explosion *> *old_explosions = explosions;
    list<Rock *> *old_rocks = rocks;
    list<Spinner *> *old_spinners = spinners;
    shake_amt = 0.0;

    /*
    list<Shot *>::iterator s;
    list<Shot *> tmpShots( *shots ); 
    for(s = tmpShots.begin(); s != tmpShots.end(); ++s ) {
        delete *s;
    }
    
    list<Explosion *>::iterator e;
    list<Explosion *> tmpExplosions( *explosions ); 
    for(e = tmpExplosions.begin(); e != tmpExplosions.end(); ++e ) {
        delete *e;
    }

    list<Rock *>::iterator r;
    list<Rock *> tmpRocks( *rocks ); 
    for(r = tmpRocks.begin(); r != tmpRocks.end(); ++r ) {
        delete *r;
    }

    list<Spinner *>::iterator sp;
    list<Spinner *> tmpSpinners( *spinners ); 
    for(sp = tmpSpinners.begin(); sp != tmpSpinners.end(); ++sp ) {
        delete *sp;
    }*/

    delete old_shots;
    delete old_explosions;
    delete old_rocks;
    delete old_spinners;
    shots = new list<Shot *>;
    rocks = new list<Rock *>;
    explosions = new list<Explosion *>;
    spinners = new list<Spinner *>;

    updateScore(DEATH_SCORE);

    int frameCount = 0;
    while(frameCount < DEATH_FRAME_COUNT) {
        ulStartDrawing2D();

        // First draw background
        ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, RED);
        SHAKE(DEATH_SHAKE_AMT*(float)(DEATH_FRAME_COUNT-frameCount)/DEATH_FRAME_COUNT);
        ulDrawImageXY( bgImg, 0, 0 );

        ulEndDrawing();
        ulSyncFrame();
        ++frameCount;
    }
    ulResetScreenView();
}

void Game::mainLoop() {

    //To avoid a divide by zero the first time
	totalTime = 1;

	while(1) {
        //Initialize the timers to measure the framerate
	    TIMER1_CR = 0;
	    TIMER1_DATA = 0;
	    TIMER1_CR = TIMER_DIV_64 | TIMER_ENABLE;

        if(RAND(50/multiplyer)==0)
            rocks->push_back(new Rock(this, (Rock *)0, 0, RAND(NUM_ROCK_IMAGES) ));
        if(RAND(200/multiplyer)==0)
            spinners->push_back(new Spinner(this));
        update();
        draw();
		totalTime = TIMER1_DATA;
	}
}


