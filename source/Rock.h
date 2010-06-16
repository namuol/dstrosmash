#ifndef _ROCK_H
#define _ROCK_H

#include <ulib/ulib.h>
#include "Game.h"

#define NUM_ROCK_IMAGES 6
#include "sm1.h"
#include "sm2.h"
#include "sm3.h"
#include "lg1.h"
#include "lg2.h"
#include "lg3.h"

#define MIN_ROCK_YSPEED 0.33
#define MAX_ROCK_YSPEED 2.0
#define MIN_ROCK_XSPEED 0
#define MAX_ROCK_XSPEED 0.25
#define ROCK_SPLIT_SPEED 0.25
#define ROCK_SPLIT_PROBABILITY 0.75


class Rock : public Sprite {
private:
    bool is_big;
    bool flipped;

public:
    static const int *ROCK_COLORS[];
    int color;
    float vx, vy;
    int rock_num;
    static UL_IMAGE *images[NUM_ROCK_IMAGES];
    static void loadImages();
    Rock(Game *game, Rock *parent, int num, int rock_num);
    ~Rock();
   
    void update();
    void draw();
    void kill(DeathType deathType);
};

#endif
