#ifndef _ROCK_H
#define _ROCK_H

#include <ulib/ulib.h>

#define NUM_ROCK_IMAGES 6
#include "sm1.h"
#include "sm2.h"
#include "sm3.h"
#include "lg1.h"
#include "lg2.h"
#include "lg3.h"

#define MIN_ROCK_YSPEED 0.5
#define MAX_ROCK_YSPEED 1.5
#define MIN_ROCK_XSPEED 0
#define MAX_ROCK_XSPEED 0.75

enum RockDeathType { 
    LAND, // When the rock hits the ground
    SHOT, // When the rock is shot
    OUT_OF_BOUNDS // When the rock goes off the screen (doesn't affect the score)
};

class Rock : public Sprite {
private:
    int color;
    float vx, vy;

public:
    static const int ROCK_COLORS[];
    static UL_IMAGE *images[NUM_ROCK_IMAGES];
    Rock(Game *game, Rock *parent, int num);
    ~Rock();
   
    void update();
    void draw();
    void kill(RockDeathType deathType);
};

#endif
