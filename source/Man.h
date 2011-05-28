#ifndef _MAN_H
#define _MAN_H

#include <ulib/ulib.h>
#include "Sprite.h"

#define MAN_HEIGHT 8
#define MAN_WIDTH 7

class Man : public Sprite {
    int next_shot;
    bool auto_fire;
    int hyper_charge;

public:
    static UL_IMAGE *image;
    static void loadImages();
    Man();
    Man(float x, float y);
    ~Man();
   
    void update();
    void shoot();
    void hyper();
};

#endif
