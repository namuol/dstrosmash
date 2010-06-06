#ifndef _SHOT_H
#define _SHOT_H

#include "Sprite.h"

#define SHOT_SPEED -3
#define SHOT_WIDTH 2
#define SHOT_HEIGHT 8

class Shot : public Sprite {
public:
    Shot(Game *game);
    ~Shot();

    void update();
    void draw();
};

#endif
