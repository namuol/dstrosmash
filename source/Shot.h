#ifndef _SHOT_H
#define _SHOT_H

#include <ulib/ulib.h>
#include <list>
class Game;
#define SHOT_SPEED -5
#define SHOT_WIDTH 4
#define SHOT_HEIGHT 16
class Shot {
private:
    int x, y;
    Game *game;

public:
   Shot(Game *game);
   ~Shot();
   
   void update();
   void draw();
};

#endif
