/*
 * Game - a Game object is an instance of an ASTROSMASH! game.
 */
#include <nds.h>
#include <maxmod9.h>

#include "Game.h"
#include "Man.h"
#include "LevelRules.h"
#include "main.h"
#include "bg.h" // The background image
#include "intro_bg.h" // The background image for the intro screen

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
UL_IMAGE * Game::introBgImg = NULL;

Game::Game() {
    intro_progress = -1; 
    next_intro_step = intro_step_length = 3000; 

    one_up_total = 0;
    score = score_display = 0;
    peak_score = 0;
    multiplyer = 1;
    speed_scale = MIN_SPEED_SCALE + ulMin(MAX_SPEED_SCALE - MIN_SPEED_SCALE,
                                          sqrt((float)score/(float)X6_LEVEL_SCORE));
    rules = &LevelRules::RULES[0];
    next_futility = 0;/*ulMax(END_FUTILITY_RATE,
                          LERP(START_FUTILITY_RATE,
                               END_FUTILITY_RATE,
                               (speed_scale-MIN_SPEED_SCALE) / 
                               (MAX_SPEED_SCALE-MIN_SPEED_SCALE)));*/

    rules = &LevelRules::RULES[0];

    next_rock = RAND_RANGE(rules->min_rock_rest,rules->max_rock_rest);
    next_spinner = RAND_RANGE(rules->min_spinner_rest,rules->max_spinner_rest);
    next_missile = RAND_RANGE(rules->min_missile_rest,rules->max_missile_rest);
    next_ufo = RAND_RANGE(rules->min_ufo_rest,rules->max_ufo_rest);

    shake_amt = 0.0;
    lives = STARTING_LIVES; 
    bgImg = ulLoadImageFilePNG((const char*)bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL2);
    bg_y_offset = -bgImg->sizeY+192;
    introBgImg = ulLoadImageFilePNG((const char*)intro_bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL4);
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
        paused = !paused;
        //ufos->push_back(new UFO(this));
        //spinners->push_back(new Spinner(this));
        //missiles->push_back(new Missile(this));
        //SFX::spinner_start();
    }

    if(ul_keys.pressed.R)
        updateScore(1000);

    if(ul_keys.pressed.L)
        missiles->push_back(new Missile(this));
        

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

    rules = multiplyer >= 0 ? &LevelRules::RULES[multiplyer-1] : &LevelRules::RULES[0];

    // "Soundtrack" //////////////////////////////////////////////////////////
    if(next_futility <= 0)
    {
        SFX::futility();

        next_futility = ulMax(END_FUTILITY_RATE,
                              LERP(START_FUTILITY_RATE,
                                   END_FUTILITY_RATE,
                                   (speed_scale-MIN_SPEED_SCALE) / 
                                   (MAX_SPEED_SCALE-MIN_SPEED_SCALE)));
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

    SFX::update();
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

    speed_scale = MIN_SPEED_SCALE + ulMin(MAX_SPEED_SCALE - MIN_SPEED_SCALE,
                                          sqrt((float)score/(float)X6_LEVEL_SCORE));
}

void Game::intro_update() {
    ulReadKeys(0);
    if(ul_keys.pressed.start)
    {
        if(intro_progress <= INTRO_STEP_COUNT-2)
            intro_progress = INTRO_STEP_COUNT-1;
        else
            intro_progress = INTRO_STEP_COUNT;
    }

    if(intro_progress < INTRO_STEP_COUNT-2)
    { 
        if( next_intro_step <= 0)
        {
            SFX::futility();
            next_intro_step = INTRO_STEP_RATE/((3.0+intro_progress)/(3.0+INTRO_STEP_COUNT-2));
            intro_step_length = next_intro_step;
            ++intro_progress;
        }

        next_intro_step -= FRAME_LENGTH_MS;
    }
    else if(intro_progress == INTRO_STEP_COUNT-2)
    {
        SFX::death();
        shake_amt = DEATH_SHAKE_AMT*1.4;
        ++intro_progress;
    }
}

void Game::intro_draw() {
    ulStartDrawing2D();

    ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, BLACK);

    if( shake_amt > 0.0 ) {
        ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, RED);
        SHAKE(shake_amt);
        shake_amt -= SHAKE_DECREASE;
    } else {
        shake_amt = 0.0;
    }

    switch(intro_progress)
    {
    default:
    case INTRO_STEP_COUNT-2:
    case INTRO_STEP_COUNT-1:
            if(shake_amt == 0)
            {
                ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, BROWN);
                ulDrawImageXY( introBgImg, 0, 0 );
                ulPrintf_xy(6, FLOOR/2+0,  "Namuol Presents");
                ulPrintf_xy(6, FLOOR/2+72, "   (a remake)  ");
                ulPrintf_xy(6, FLOOR/2+128,"Crap @ 2010 Nam");
            }
            ulPrintf_xy(6, FLOOR/2+48, "  DSTROSMASH!  ");
        break;
    case 16:ulPrintf_xy(6, FLOOR/2+64, "     UNIVERSE. ");
    case 15:ulPrintf_xy(6, FLOOR/2+64, " THE           ");
    case 14:ulPrintf_xy(6, FLOOR/2+48, "             OF");
    case 13:ulPrintf_xy(6, FLOOR/2+48, "        ENCE   ");
    case 12:ulPrintf_xy(6, FLOOR/2+48, "     FER       ");
    case 11:ulPrintf_xy(6, FLOOR/2+48, "  DIF          ");
    case 10:ulPrintf_xy(6, FLOOR/2+48, "IN             ");
    case  9:ulPrintf_xy(6, FLOOR/2+32, "             ED");
    case  8:ulPrintf_xy(6, FLOOR/2+32, "          JAD  ");
    case  7:ulPrintf_xy(6, FLOOR/2+32, "      THE      ");
    case  6:ulPrintf_xy(6, FLOOR/2+32, " FROM          ");
    case  5:ulPrintf_xy(6, FLOOR/2+16, "           SELF");
    case  4:ulPrintf_xy(6, FLOOR/2+16, "       YOUR    ");
    case  3:ulPrintf_xy(6, FLOOR/2+16, "  FEND         ");
    case  2:ulPrintf_xy(6, FLOOR/2+16, "DE             ");
    case  1:
    case  0:
    case -1:
        ulDrawImageXY( bgImg, 0,
                            (int)(LERP((bg_y_offset)/(INTRO_STEP_COUNT-1),0,
                            ((float)next_intro_step-(float)intro_step_length) / 
                                (float)intro_step_length) +
                            LERP(0,(bg_y_offset),(float)(intro_progress)/(float)(INTRO_STEP_COUNT-1))));
        break;
    }
    ulEndDrawing(); 
    swiWaitForVBlank();
    ulResetScreenView();
}

void Game::draw() {
    char lives_str[16];

    ulStartDrawing2D();

    // First draw background
    ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, LEVEL_COLORS[lives<0?0:multiplyer]);

    //int cpuTime = (TIMER1_DATA * 1000) / totalTime;
    //ulPrintf_xy(0, 0, "CPU: %02i.%i%% %f %f", cpuTime / 10, cpuTime % 10, SHAKE_MAX, 
    //    missiles->size() == 1 ? missiles->front()->x:missiles->size());

    if( shake_amt > 0.0 ) {
        SHAKE(MIN((int)shake_amt,(int)SHAKE_MAX));
        shake_amt -= SHAKE_DECREASE;
    } else {
        shake_amt = 0.0;
    }
    ulDrawImageXY( bgImg, 0, bg_y_offset );


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
        ulDrawImageXY( bgImg, 0, bg_y_offset );

        ulEndDrawing();
        //ulSyncFrame();
        swiWaitForVBlank();
        ++frameCount;
    }
    ulResetScreenView();
}

void Game::mainLoop() {
    consoleDemoInit();

    while(intro_progress < INTRO_STEP_COUNT)
    {
        intro_update();
        intro_draw();
    }

    //To avoid a divide by zero the first time
	totalTime = 1;

	while(1) {
        //Initialize the timers to measure the framerate
	    TIMER1_CR = 0;
	    TIMER1_DATA = 0;
	    TIMER1_CR = TIMER_DIV_64 | TIMER_ENABLE;

        update();
        draw();
        //#define DEBUG
        #ifdef DEBUG
        iprintf("\x1b[0;0H                  ");
        iprintf("\x1b[1;0H                  ");
        iprintf("\x1b[2;0H                  ");
        iprintf("\x1b[3;0H                  ");
        iprintf("\x1b[4;0H                  ");
        iprintf("\x1b[5;0H                  ");
		iprintf("\x1b[0;0Hfutility_h = %d", SFX::futility_h);
		iprintf("\x1b[1;0Hhit_h = %d", SFX::hit_h);
		iprintf("\x1b[2;0Hspinner_h = %d", SFX::spinner_h);
		iprintf("\x1b[3;0Hmissile_h = %d", SFX::missile_h);
		iprintf("\x1b[4;0Hufo_h = %d", SFX::ufo_h);
		iprintf("\x1b[5;0Hdeath_h = %d", SFX::death_h);
        #endif

		totalTime = TIMER1_DATA;
	}
}

const LevelRules* Game::getRules() const {
    return rules;
}
