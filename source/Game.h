#ifndef _GAME_H
#define _GAME_H

#define LEFT_WALL 0
#define RIGHT_WALL 127
#define FLOOR 85
#define CEILING 0
#define MAN_SPEED 2
#define MAX_SHOTS 2
#define MAX_ROCKS 16
#define MAX_EXPLOSIONS 16
#define MAX_SPINNERS 5
#define MAX_MISSILES 2
#define MAX_UFOS 1
#define MAX_UFO_SHOTS 10

#define MIN_SPEED_SCALE 0.3
#define MAX_SPEED_SCALE 1.0

#define BIG_ROCK_SHOT_SCORE 10
#define SMALL_ROCK_SHOT_SCORE 20
#define BIG_SPINNER_SHOT_SCORE 40
#define SMALL_SPINNER_SHOT_SCORE 80
#define MISSILE_SHOT_SCORE 50
#define UFO_SHOT_SCORE 100
#define BIG_ROCK_LAND_SCORE -5
#define SMALL_ROCK_LAND_SCORE -10
#define BIG_SPINNER_LAND_SCORE 0 // Death is enough of a panalty.
#define SMALL_SPINNER_LAND_SCORE 0 // Death is enough of a penalty.
#define MISSILE_LAND_SCORE 0
#define DEATH_SCORE -100 

#define FRAME_LENGTH_MS 17

#define NUM_COLORS 16

#define BLACK RGB15(0,0,0)
#define BLUE RGB15(0,5,31)
#define RED RGB15(31,7,2)
#define OFF_WHITE RGB15(25,25,21)
#define DARK_GREEN RGB15(7,13,7)
#define GREEN RGB15(0,20,10)
#define YELLOW RGB15(31,29,10)
#define WHITE RGB15(31,31,31)
#define GRAY RGB15(23,21,25)
#define CYAN RGB15(4,22,31)
#define ORANGE RGB15(31,22,3)
#define BROWN RGB15(10,13,0)
#define PINK RGB15(31,9,10)
#define LIGHT_BLUE RGB15(20,18,31)
#define YELLOW_GREEN RGB15(14,25,16)
#define PURPLE RGB15(22,3,11)

// Color flags; useful for limiting
//  what colors certain sprites can be.
// OR them together to choose colors,
//  AND with them to test.
#define BLACK_EN        1
#define BLUE_EN         2
#define RED_EN          4
#define OFF_WHITE_EN    8
#define DARK_GREEN_EN   16
#define GREEN_EN        32
#define YELLOW_EN       64
#define WHITE_EN        128
#define GRAY_EN         256
#define CYAN_EN         512
#define ORANGE_EN       1024
#define BROWN_EN        2048
#define PINK_EN         4096
#define LIGHT_BLUE_EN   8192
#define YELLOW_GREEN_EN 16384
#define PURPLE_EN       32768

#define MAX_MULTIPLYER 6
#define X2_LEVEL_SCORE 1000
#define X3_LEVEL_SCORE 5000
#define X4_LEVEL_SCORE 20000
#define X5_LEVEL_SCORE 50000
#define X6_LEVEL_SCORE 100000

#define STARTING_LIVES 4
#define ONE_UP_SCORE 6000 // Every one of these gets you +1 life.

#define DEATH_FRAME_COUNT 130
#define DEATH_SHAKE_AMT 10.0
#define SHAKE_MAX 0.0

#define SHAKE_DECREASE 0.1
#define ROCK_LAND_SHAKE 2.0

#define START_FUTILITY_RATE 800
#define END_FUTILITY_RATE 400

#define INTRO_STEP_RATE 300
#define INTRO_STEP_COUNT 19

#define DEATH_DEBRIS_COUNT 5
#define PI 3.14159265
#define DEATH_DEBRIS_ANG (PI/(DEATH_DEBRIS_COUNT-1))
#define DEATH_DEBRIS_SPEED 0.4

#include <ctime>
#include <cstdlib>
#include <list>
#include "vector.h"

#include <maxmod9.h>

#include "util.h"

#include "Man.h"
#include "Shot.h"
#include "Rock.h"
#include "Explosion.h"
#include "Spinner.h"
#include "UFO.h"
#include "UFOShot.h"
#include "Missile.h"
#include "Debris.h"
#include "LevelRules.h"

#define game (Game::inst())

class Game {
private:
    static Game* _inst;
    static const int LEVEL_COLORS[];
    int totalTime;
	static UL_IMAGE *bgImg;
	static UL_IMAGE *introBgImg;
    void intro_update();
    void intro_draw();
    void update();
    void draw();
    int lives;
    float score_display; // Number which slides up and down for kicks.
    
    LevelRules* rules;

    // Countdowns until next enemy (of each type):
    int next_rock;
    int next_spinner;
    int next_missile;
    int next_ufo;

    int next_futility;
    
    int intro_progress;
    int next_intro_step;
    int intro_step_length;
    int bg_y_offset;

    bool paused;
    bool done;
    bool just_died;

protected:
    Game();
    ~Game();

public:
    int sprite_count;

    float shake_amt;
    int multiplyer;
    float speed_scale;
    int score;
    int peak_score;
    int one_up_total;

    Man theMan;
    vector<Shot, MAX_SHOTS> shots;
    vector<Rock, MAX_ROCKS> rocks;
    vector<Explosion, MAX_EXPLOSIONS> explosions;
    vector<Spinner, MAX_SPINNERS> spinners;
    vector<Missile, MAX_MISSILES> missiles;
    vector<UFO, MAX_UFOS> ufos;
    vector<UFOShot, MAX_UFO_SHOTS> ufo_shots;
    vector<Debris, DEATH_DEBRIS_COUNT> debris;

    static Game* inst();

    const LevelRules* getRules() const;

    void init();   
    void mainLoop();
    void updateScore(int amount);
    void death();
};


#endif
