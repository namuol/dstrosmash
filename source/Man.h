#ifndef _MAN_H
#define _MAN_H

#include "Sprite.h"

#define MAN_HEIGHT 8
#define MAN_WIDTH 7

class Man : public Sprite {
public:
    Man(Game *game, float x, float y);
    ~Man();
   
    void update();
    void shoot();
};

#endif
