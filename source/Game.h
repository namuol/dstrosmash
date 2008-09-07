#ifndef _GAME_H
#define _GAME_H

#include <ctime>
#include <cstdlib>
#include <list>

#include "util.h"

#include "Man.h"
#include "Shot.h"
#include "Explosion.h"
#include "Rock.h"

#define LEFT_WALL 0
#define RIGHT_WALL 127
#define FLOOR 85
#define CEILING 0
#define MAN_SPEED 2
#define MAX_SHOTS 2

#define BIG_ROCK_SHOT_SCORE 10
#define SMALL_ROCK_SHOT_SCORE 20
#define BIG_SPINNER_SHOT_SCORE 40
#define SMALL_SPINNER_SHOT_SCORE 80
#define GUIDED_MISSILE_SHOT_SCORE 50
#define UFO_SHOT_SCORE 100
#define BIG_ROCK_LAND_SCORE -5
#define SMALL_ROCK_LAND_SCORE -10
#define DEATH_SCORE -100 

#define X2_LEVEL 1000
#define X3_LEVEL 5000
#define X4_LEVEL 20000
#define X5_LEVEL 50000
#define X6_LEVEL 100000

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

class Game {
private:
    int totalTime;
	static UL_IMAGE *bgImg;
    void update();
    void draw();

public:
    int multiplyer;
    int score;
    int peak_score;

    Man *theMan;
    std::list<Shot *> shots;
    std::list<Rock *> rocks;
    std::list<Explosion *> explosions;
    Game();
    ~Game();
   
    void mainLoop();
    void updateScore(int amount);
};


#endif
