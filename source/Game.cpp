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

//#define DEBUG
#ifdef DEBUG
#define print_debug_info() \
    iprintf("\x1b[0;0H                  ");\
    iprintf("\x1b[1;0H                  ");\
    iprintf("\x1b[2;0H                  ");\
    iprintf("\x1b[3;0H                  ");\
    iprintf("\x1b[4;0H                  ");\
    iprintf("\x1b[5;0H                  ");\
    iprintf("\x1b[6;0H                  ");\
    iprintf("\x1b[7;0H                  ");\
    iprintf("\x1b[8;0H                  ");\
    iprintf("\x1b[9;0H                  ");\
    iprintf("\x1b[10;0H                  ");\
    iprintf("\x1b[11;0H                  ");\
    iprintf("\x1b[0;0Hfutility_h = %d", SFX::futility_h);\
    iprintf("\x1b[1;0Hhit_h = %d", SFX::hit_h);\
    iprintf("\x1b[2;0Hspinner_h = %d", SFX::spinner_h);\
    iprintf("\x1b[3;0Hmissile_h = %d", SFX::missile_h);\
    iprintf("\x1b[4;0Hufo_h = %d", SFX::ufo_h);\
    iprintf("\x1b[5;0Hdeath_h = %d", SFX::death_h);\
    iprintf("\x1b[6;0Hsprite_count = %d", sprite_count);\
    iprintf("\x1b[7;0Hrocks.size() = %d", rocks.size());\
    iprintf("\x1b[10;0Hmissiles.size() = %d", missiles.size());\
    iprintf("\x1b[11;0Hspinners.size() = %d", spinners.size());
    //iprintf("\x1b[11;0Hufos.size() = %d", ufos.size());
    //iprintf("\x1b[8;0Hshots.size() = %d", shots.size());
#else
#define print_debug_info() 
#endif

#define HANDLE_SWAP_BUTTON() if(ul_keys.pressed.L) lcdSwap();
#define HANDLE_PAUSE_BUTTON() {\
    if(ul_keys.pressed.start) {\
        paused = !paused;\
        if(paused)\
            SFX::mute();\
        else\
            SFX::unmute();\
    }}

Game* Game::_inst = NULL;

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
    bgImg = ulLoadImageFilePNG((const char*)bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL2);
    introBgImg = ulLoadImageFilePNG((const char*)intro_bg, (int)bg_size, UL_IN_VRAM, UL_PF_PAL4);
    theMan=Man(RIGHT_WALL/2, FLOOR-MAN_HEIGHT);	
}

Game::~Game() {
    delete bgImg;
}

void Game::init() {
    theMan.x = RIGHT_WALL/2;
    sprite_count = 0;
    intro_progress = -1; 
    next_intro_step = intro_step_length = 3000; 

    one_up_total = 0;
    score = score_display = 0;
    peak_score = 0;
    multiplyer = 1;
    speed_scale = MIN_SPEED_SCALE + ulMin(MAX_SPEED_SCALE - MIN_SPEED_SCALE,
                                          sqrt((float)score/(float)X6_LEVEL_SCORE));
    rules = &LevelRules::RULES[0];
    next_futility = 0;

    rules = &LevelRules::RULES[0];

    next_rock = RAND_RANGE(rules->min_rock_rest,rules->max_rock_rest);
    next_spinner = RAND_RANGE(rules->min_spinner_rest,rules->max_spinner_rest);
    next_missile = RAND_RANGE(rules->min_missile_rest,rules->max_missile_rest);
    next_ufo = RAND_RANGE(rules->min_ufo_rest,rules->max_ufo_rest);

    shake_amt = 0.0;
    lives = STARTING_LIVES; 
    bg_y_offset = -bgImg->sizeY+192;

    paused = false;
    done = false;
    just_died = false;

    rocks.deinit_clear();
    spinners.deinit_clear();
    missiles.deinit_clear();
    ufos.deinit_clear();
    ufo_shots.deinit_clear();
    debris.deinit_clear();

    SFX::unmute();
}

Game* Game::inst() {
    if(_inst==NULL)
        _inst = new Game();
    return _inst;
}

void Game::update() {
	//Read keys
	ulReadKeys(0);

    if(lives < 0) { // If GAME OVER
        if(ul_keys.pressed.start)
            done=true;
    } else {
        HANDLE_PAUSE_BUTTON();
    }

    just_died = false; // Reset the flag.

    if(paused)
        return;

    if( lives >= 0 )
    {
        theMan.update();

        // Update each shot.
        for(unsigned int i=0; i < shots.capacity(); ++i)
            if(shots.active(i)) shots[i].update();
    }

    // Update each rock.
    for(unsigned int i=0; i < rocks.capacity(); ++i)
        if(rocks.active(i)) rocks[i].update();

    // Update each spinner.
    for(unsigned int i=0; i < spinners.capacity(); ++i)
        if(spinners.active(i)) spinners[i].update();

    // Update each ufo.
    for(unsigned int i=0; i < ufos.capacity(); ++i)
        if(ufos.active(i)) ufos[i].update();

    // Update each ufo_shot.
    for(unsigned int i=0; i < ufo_shots.capacity(); ++i)
        if(ufo_shots.active(i)) ufo_shots[i].update();

    // Update each missile.
    for(unsigned int i=0; i < missiles.capacity(); ++i)
        if(missiles.active(i)) missiles[i].update();

    // Update each explosion.
    for(unsigned int i=0; i < explosions.capacity(); ++i)
        if(explosions.active(i)) explosions[i].update();

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

    if(next_rock <= 0 && rocks.size() < rules->max_rocks)
    {
        int to_spawn = ulMin(RAND(rules->max_rock_spawn) + 1,
                            rules->max_rocks - rocks.size());
        for(int i=0; i < to_spawn; ++i)
        {
            int id = rocks.add();
            if(id >= 0)
                rocks[id].init(id, (Rock *)NULL, 0, RAND(NUM_ROCK_IMAGES));
        }
        next_rock = RAND_RANGE(rules->min_rock_rest,rules->max_rock_rest);
    }

    if(next_spinner <= 0 && spinners.size() < rules->max_spinners)
    {
        int to_spawn = ulMin(RAND(rules->max_spinner_spawn) + 1,
                            rules->max_spinners - spinners.size());
        for(int i=0; i < to_spawn; ++i)
        {
            int id = spinners.add();
            if(id >= 0)
                spinners[id].init(id);
        }
        next_spinner = RAND_RANGE(rules->min_spinner_rest,rules->max_spinner_rest);
    }

    if(next_missile <= 0 && missiles.size() < rules->max_missiles)
    {
        int id = missiles.add();
        if(id >= 0)
            missiles[id].init(id);
        next_missile = RAND_RANGE(rules->min_missile_rest,rules->max_missile_rest);
    }
    
    if(next_ufo <= 0 && ufos.size() < rules->max_ufos)
    {
        int id = ufos.add();
        if(id >= 0)
            ufos[id].init(id);
        next_ufo = RAND_RANGE(rules->min_ufo_rest,rules->max_ufo_rest);
    }

    if( ufos.size() <= 0 ) {
        next_rock -= FRAME_LENGTH_MS;
        next_spinner -= FRAME_LENGTH_MS;
        if(rules->max_ufos > 0)
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

    if(score < 0)
        speed_scale = MIN_SPEED_SCALE;
    else
        speed_scale = MIN_SPEED_SCALE + ulMin(MAX_SPEED_SCALE - MIN_SPEED_SCALE,
                                          sqrt((float)score/(float)X6_LEVEL_SCORE));
}

void Game::intro_update() {
    ulReadKeys(0);
    if(ul_keys.pressed.start)
    {
        SFX::mute();
        SFX::unmute();
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

    if (lives < 0) {
        ulPrintf_xy(28, FLOOR/2, "GAME OVER");
        ulPrintf_xy(28, FLOOR/2+16, "Final Score:");
        ulPrintf_xy(28, FLOOR/2+32, "%12i", score);
        ulPrintf_xy(28, FLOOR/2+48, " Peak Score:");
        ulPrintf_xy(28, FLOOR/2+64, "%12i", peak_score);
    }

    // Draw all the shots:
    for(unsigned int i=0; i < shots.capacity(); ++i)
        if(shots.active(i)) shots[i].draw();

    // Draw all the rocks:
    for(unsigned int i=0; i < rocks.capacity(); ++i)
        if(rocks.active(i)) rocks[i].draw();

    // Draw all the spinners:
    for(unsigned int i=0; i < spinners.capacity(); ++i)
        if(spinners.active(i)) spinners[i].draw();

    // Draw all the missiles:
    for(unsigned int i=0; i < missiles.capacity(); ++i)
        if(missiles.active(i)) missiles[i].draw();

    // Draw all the ufos:
    for(unsigned int i=0; i < ufos.capacity(); ++i)
        if(ufos.active(i)) ufos[i].draw();

    // Draw all the ufo_shots:
    for(unsigned int i=0; i < ufo_shots.capacity(); ++i)
        if(ufo_shots.active(i)) ufo_shots[i].draw();

    // Draw all the explosions:
    for(unsigned int i=0; i < explosions.capacity(); ++i)
        if(explosions.active(i)) explosions[i].draw();


    score_display += ((score - score_display)/5.0)+(score>0?0.1:-0.1);

    if( lives >= 0 ) {
        theMan.draw();
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

    //print_debug_info();

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
    if(just_died) return;
    // This gets called each time the player dies. It should be called within
    // the update() call in mainLoop.
    //  It enters it's own loop to display the death animation and 
    //  then returns to the mainLoop in a natural fashion.
    if(lives < 0)
        return;

    SFX::death();
    --lives;
    if(lives < 0)
        SFX::muted=true;
    shake_amt = 0.0;

    shots.deinit_clear();
    rocks.deinit_clear();
    explosions.deinit_clear();
    spinners.deinit_clear();
    missiles.deinit_clear();
    ufos.deinit_clear();
    ufo_shots.deinit_clear();

    for(int i=0;i<DEATH_DEBRIS_COUNT; ++i) {
        int id = debris.add();
        debris[id].init(id,
            theMan.x,
            theMan.y+4,
            cos(i*DEATH_DEBRIS_ANG-PI)*DEATH_DEBRIS_SPEED,
            sin(i*DEATH_DEBRIS_ANG-PI)*DEATH_DEBRIS_SPEED);
    }

    updateScore(DEATH_SCORE);

    int frameCount = 0;

    while(frameCount < DEATH_FRAME_COUNT) {
        ulReadKeys(0);
        HANDLE_PAUSE_BUTTON();
        HANDLE_SWAP_BUTTON();
        if(paused) continue;

        ulStartDrawing2D();
        // First draw background
        ulDrawFillRect(0, 0, UL_SCREEN_WIDTH, UL_SCREEN_HEIGHT, RED);
        SHAKE(DEATH_SHAKE_AMT*(float)(DEATH_FRAME_COUNT-frameCount)/DEATH_FRAME_COUNT);
        ulDrawImageXY( bgImg, 0, bg_y_offset );

        for(int i=0;i<DEATH_DEBRIS_COUNT;++i) {
            debris[i].update();
            debris[i].draw();
        }
        
        ulEndDrawing();
        print_debug_info();

        swiWaitForVBlank();
        ++frameCount;
    }

    debris.deinit_clear();
    ulResetScreenView();
}

void Game::mainLoop() {

    while(intro_progress < INTRO_STEP_COUNT)
    {
        intro_update();
        HANDLE_SWAP_BUTTON();
        intro_draw();
        print_debug_info();
    }

    //To avoid a divide by zero the first time
	totalTime = 1;

	while(!done) {
        //Initialize the timers to measure the framerate
	    TIMER1_CR = 0;
	    TIMER1_DATA = 0;
	    TIMER1_CR = TIMER_DIV_64 | TIMER_ENABLE;

        update();
        HANDLE_SWAP_BUTTON();
        draw();

		totalTime = TIMER1_DATA;
	}
}

const LevelRules* Game::getRules() const {
    return rules;
}
