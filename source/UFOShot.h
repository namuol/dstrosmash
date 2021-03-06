#ifndef _UFOSHOT_H
#define _UFOSHOT_H

#include <ulib/ulib.h>
#include "Game.h"
#include "UFO.h"

class UFOShot : public Sprite {
public:
    float vx, vy;
    static UL_IMAGE *image;
    static void loadImages();
    void init(int id, const UFO* parent);
   
    void update();
    void draw();
    void kill(DeathType deathType);
};

#endif
