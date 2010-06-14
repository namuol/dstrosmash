/*
 * Game - a Game object is an instance of an ASTROSMASH! game.
 */
#include <maxmod9.h>

#include "Game.h"
#include "Man.h"
#include "LevelRules.h"
#include "main.h"
#include "bg.h" // The background image

#include <list>
using namespace std;

#include "soundbank.h"
#include "sfx.h"

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
    score = score_display = 0;
    peak_score = 0;
    multiplyer = 1;
    speed_scale = sqrt((float)multiplyer/(float)MAX_MULTIPLYER);
    rules = &X1_RULES;
    
    next_futility = LERP(START_FUTILITY_RATE,
                         END_FUTILITY_RATE,
                         score / X6_LEVEL_SCORE);

    next_rock = RAND_RANGE(rules->min_rock_rest,rules->max_rock_rest);
    next_spinner = RAND_RANGE(rules->min_spinner_rest,rules->max_spinner_rest);
    next_missile = RAND_RANGE(rules->min_missile_rest,rules->max_missile_rest);
    next_ufo = RAND_RANGE(rules->min_ufo_rest,rules->max_ufo_rest);

    shake_amt = 0.0;
    lives = STARTING_LIVES; 
    bgImg = ulLoadImageFilePNG((const char*)bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL2);
    theMan = new Man(this, 32, FLOOR-MAN_HEIGHT);	

    shots = new list<Shot *>;
    rocks = new list<Rock *>;
    explosions = new list<Explosion *>;
    spinners = new list<Spinner *>;
    missiles = new list<Missile *>;
    ufos = new list<UFO *>;
    ufo_shots = new list<UFOShot *>;
    
    paused = false;
}

Game::~Game() {
    delete theMan;
    delete bgImg;
    delete shots;
    delete rocks;
    delete explosions;
    delete spinners;
    delete ufos;
    delete ufo_shots;
    delete missiles;
}

void Game::update() {
	//Read keys
	ulReadKeys(0);

    if(ul_keys.pressed.start)
    {
        //paused = !paused;
        //ufos->push_back(new UFO(this));
        //spinners->push_back(new Spinner(this));
        missiles->push_back(new Missile(this));
    }

    if(paused)
        return;

    if( lives >= 0 )
    {
        theMan->update();

        //  We need to use a temporary list to prevent invalidating the 
        //  iterator when deleting a shot that goes off the screen or collides with something.
        // Update each shot.
        list<Shot *>::iterator s;
        list<Shot *> tmpShots( *shots ); 
        for(s = tmpShots.begin(); s != tmpShots.end(); ++s ) {
            (*s)->update();
        }
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

    // Update each ufo.
    list<UFO *>::iterator u;
    list<UFO *> tmpUFOs( *ufos ); 
    for(u = tmpUFOs.begin(); u != tmpUFOs.end(); ++u ) {
        (*u)->update();
    }

    // Update each ufo_shot.
    list<UFOShot *>::iterator us;
    list<UFOShot *> tmpUFOShots( *ufo_shots ); 
    for(us = tmpUFOShots.begin(); us != tmpUFOShots.end(); ++us ) {
        (*us)->update();
    }

    // Update each missile.
    list<Missile *>::iterator m;
    list<Missile *> tmpMissiles( *missiles ); 
    for(m = tmpMissiles.begin(); m != tmpMissiles.end(); ++m ) {
        (*m)->update();
    }

    // Update each explosion.
    list<Explosion *>::iterator e;
    list<Explosion *> tmpExplosions( *explosions ); 
    for(e = tmpExplosions.begin(); e != tmpExplosions.end(); ++e ) {
        (*e)->update();
    }


    switch(multiplyer)
    {
    case 1: rules = &X1_RULES; break;
    case 2: rules = &X2_RULES; break;
    case 3: rules = &X3_RULES; break;
    case 4: rules = &X4_RULES; break;
    case 5: rules = &X5_RULES; break;
    case 6: rules = &X6_RULES; break;
    default: rules = &X1_RULES; break;
    }

    // "Soundtrack" //////////////////////////////////////////////////////////
    if(next_futility <= 0)
    {
        SFX::futility();
        next_futility = LERP(START_FUTILITY_RATE,
                             END_FUTILITY_RATE,
                             score / X6_LEVEL_SCORE);
    }
    next_futility -= FRAME_LENGTH_MS;

    // Enemy spawning: ///////////////////////////////////////////////////////

    if(next_rock <= 0 && rocks->size() < rules->max_rocks)
    {
        int to_spawn = ulMin(RAND(rules->max_rock_spawn) + 1,
                            rules->max_rocks - rocks->size());
        for(int i=0; i < to_spawn; ++i)
            rocks->push_back(new Rock(this, (Rock *)NULL, 0, RAND(NUM_ROCK_IMAGES) ));
        next_rock = RAND_RANGE(rules->min_rock_rest,rules->max_rock_rest);
    }

    if(next_spinner <= 0 && spinners->size() < rules->max_spinners)
    {
        int to_spawn = ulMin(RAND(rules->max_spinner_spawn) + 1,
                            rules->max_spinners - spinners->size());
        for(int i=0; i < to_spawn; ++i)
            spinners->push_back(new Spinner(this));
        next_spinner = RAND_RANGE(rules->min_spinner_rest,rules->max_spinner_rest);
    }

    if(next_missile <= 0 && missiles->size() < rules->max_missiles)
    {
        missiles->push_back(new Missile(this));
        next_missile = RAND_RANGE(rules->min_missile_rest,rules->max_missile_rest);
    }

    if(next_ufo <= 0 && ufos->size() < rules->max_ufos)
    {
        ufos->push_back(new UFO(this));
        next_ufo = RAND_RANGE(rules->min_ufo_rest,rules->max_ufo_rest);
    }

    if( ufos->size() <= 0 ) {
        next_rock -= FRAME_LENGTH_MS;
        next_spinner -= FRAME_LENGTH_MS;
        next_ufo -= FRAME_LENGTH_MS;
        next_missile -= FRAME_LENGTH_MS;
    }
}

void Game::updateScore(int amount) {
    if( lives < 0 )
        return;

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

    speed_scale = sqrt((float)multiplyer/(float)MAX_MULTIPLYER);
}

void Game::draw() {
    char lives_str[16];

    ulStartDrawing2D();

    // First draw background
    ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, LEVEL_COLORS[lives<0?0:multiplyer]);

    int cpuTime = (TIMER1_DATA * 1000) / totalTime;
    //ulPrintf_xy(0, 0, "CPU: %02i.%i%% %f %f", cpuTime / 10, cpuTime % 10, SHAKE_MAX, 
    //    missiles->size() == 1 ? missiles->front()->x:missiles->size());

    if( shake_amt > 0.0 ) {
        SHAKE(MIN((int)shake_amt,(int)SHAKE_MAX));
        shake_amt -= SHAKE_DECREASE;
    } else {
        shake_amt = 0.0;
    }
    ulDrawImageXY( bgImg, 0, 0 );


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

    // Draw all the missiles:
    std::list<Missile *>::iterator m;
    for(m = missiles->begin(); m != missiles->end(); ++m ) {
        (*m)->draw();
    }

    // Draw all the ufos:
    std::list<UFO *>::iterator u;
    for(u = ufos->begin(); u != ufos->end(); ++u ) {
        (*u)->draw();
    }

    // Draw all the ufo_shots:
    std::list<UFOShot *>::iterator us;
    for(us = ufo_shots->begin(); us != ufo_shots->end(); ++us ) {
        (*us)->draw();
    }

    // Draw all the explosions:
    list<Explosion *>::iterator e;
    for(e = explosions->begin(); e != explosions->end(); ++e ) {
        (*e)->draw();
    }


    score_display += ((score - score_display)/5.0)+0.1;

    if( lives >= 0 ) {
        theMan->draw();
    } else {
        ulPrintf_xy(28, FLOOR/2, "GAME OVER");
        ulPrintf_xy(28, FLOOR/2+16, "Final Score:");
        ulPrintf_xy(28, FLOOR/2+32, "%12i", score);
        ulPrintf_xy(28, FLOOR/2+48, " Peak Score:");
        ulPrintf_xy(28, FLOOR/2+64, "%12i", peak_score);
    }
    if( lives > 6 )
        sprintf(lives_str, "*%-6i", lives);
    else
    {
        int i;
        for(i=0; i < 6; ++i)
            lives_str[i] = i < lives ? '*' : ' ';

        lives_str[i] = NULL;
    }
        
    ulPrintf_xy(0, FLOOR*2+4, "%7i %7ix", (int)score_display, multiplyer);
    ulSetTextColor( GREEN );
    ulPrintf_xy(0, FLOOR*2+4, "        %s", lives_str);
    ulSetTextColor( WHITE );

	//End the drawing
	ulEndDrawing();

    //ulDrawLine(LEFT_WALL*2, 0, LEFT_WALL*2, CEILING*2, RGB15(31,0,0));
    //ulDrawLine(RIGHT_WALL*2, 0, RIGHT_WALL*2, CEILING*2, RGB15(31,0,0));
		
	//Wait the VBlank (synchronize at 60 fps)
	//ulSyncFrame();
    swiWaitForVBlank();
    ulResetScreenView();
}

void Game::death() {
    // This gets called each time the player dies. It should be called within
    // the update() call in mainLoop.
    //  It enters it's own loop to display the death animation and 
    //  then returns to the mainLoop in a natural fashion.
    if(lives < 0)
        return;

    SFX::death();
    --lives;
    if(lives < 0)
        SFX::muted = true;
    list<Shot *> *old_shots = shots;
    list<Explosion *> *old_explosions = explosions;
    list<Rock *> *old_rocks = rocks;
    list<Spinner *> *old_spinners = spinners;
    list<Missile *> *old_missiles = missiles;
    list<UFO *> *old_ufos = ufos;
    list<UFOShot *> *old_ufo_shots = ufo_shots;
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
    delete old_missiles;
    delete old_ufos;
    delete old_ufo_shots;
    shots = new list<Shot *>;
    rocks = new list<Rock *>;
    explosions = new list<Explosion *>;
    spinners = new list<Spinner *>;
    missiles = new list<Missile *>;
    ufos = new list<UFO *>;
    ufo_shots = new list<UFOShot *>;

    updateScore(DEATH_SCORE);

    int frameCount = 0;
    while(frameCount < DEATH_FRAME_COUNT) {
        ulStartDrawing2D();

        // First draw background
        ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, RED);
        SHAKE(DEATH_SHAKE_AMT*(float)(DEATH_FRAME_COUNT-frameCount)/DEATH_FRAME_COUNT);
        ulDrawImageXY( bgImg, 0, 0 );

        ulEndDrawing();
        //ulSyncFrame();
        swiWaitForVBlank();
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

        update();
        draw();
		totalTime = TIMER1_DATA;
	}
}

const LevelRules* Game::getRules() const {
    return rules;
}
