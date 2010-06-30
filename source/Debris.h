#ifndef _DEBRIS_H
#define _DEBRIS_H

#include "Sprite.h"

class Debris : public Sprite {
private:
    float vx, vy;
public:
    void init(int id, float x, float y, float vx, float vy);
    void update();
    void draw();
};

#endif
