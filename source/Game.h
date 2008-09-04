#ifndef _GAME_H
#define _GAME_H

#include <ctime>
#include <cstdlib>
#include <list>

#include "util.h"

#include "Man.h"
#include "Shot.h"
#include "Rock.h"

#define WALL_LEFT 0
#define WALL_RIGHT 255
#define FLOOR 162
#define CEILING 0
#define MAN_SPEED 3
#define MAX_SHOTS 2


class Game {
private:
	static UL_IMAGE *bgImg;
    void update();
    void draw();

public:
    Man *theMan;
    std::list<Shot *> shots;
    std::list<Rock *> rocks;
    Game();
    ~Game();
   
    void mainLoop();
};


#endif
