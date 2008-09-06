#ifndef _GAME_H
#define _GAME_H

#include <ctime>
#include <cstdlib>
#include <list>

#include "util.h"

#include "Man.h"
#include "Shot.h"
#include "Rock.h"

#define LEFT_WALL 0
#define RIGHT_WALL 127
#define FLOOR 85
#define CEILING 0
#define MAN_SPEED 1.5
#define MAX_SHOTS 2


class Game {
private:
    int totalTime;
	static UL_IMAGE *bgImg;
    void update();
    void draw();

public:
    static const int INTV_PALETTE[];
    Man *theMan;
    std::list<Shot *> shots;
    std::list<Rock *> rocks;
    Game();
    ~Game();
   
    void mainLoop();
};


#endif
