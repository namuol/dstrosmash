#ifndef _MAN_H
#define _MAN_H

#include <ulib/ulib.h>
#include <list>
#include "man.h"

class Game;

class Man {
private:
	static UL_IMAGE *img;
    Game *game;

public:
    float x, y;
    Man(Game *game, float x, float y);
    ~Man();
   
    void update();
    void draw();
    void shoot();
};

#endif
