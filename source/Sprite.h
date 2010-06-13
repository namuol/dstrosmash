#ifndef _SPRITE_H
#define _SPRITE_H

#include <ulib/ulib.h>

#define BOTTOM(s) ((s)->y+(s)->h)
#define TOP(s) ((s)->y)
#define LEFT(s) ((s)->x)
#define RIGHT(s) ((s)->x+(s)->w)
#define MIDX(s) ((s)->x+(s)->w/2)
#define MIDY(s) ((s)->y+(s)->h/2)

// Who's ever heard of a safe *and* useful macro, anyway?
#define COLTEST(s1, s2) !(BOTTOM(s1) < TOP(s2) || TOP(s1) > BOTTOM(s2) || RIGHT(s1) < LEFT(s2) || LEFT(s1) > RIGHT(s2))

enum DeathType { 
    LAND, // When the rock hits the ground
    SHOT, // When the rock is shot
    COLLIDE, // When the sprite collides with another sprite
    EXPLODED, // When the rock is near an explosion
    OUT_OF_BOUNDS // When the rock goes off the screen (doesn't affect the score)
};

class Game;

class Sprite {
protected:
    int imgx, imgy; // The actual on-screen coordinates of the sprite.
	UL_IMAGE *img;
    Game *game;

public:
    int w, h; // Width/height in pixels.
    float x, y;
    Sprite(Game *game, UL_IMAGE *img, float x, float y);
    Sprite(Game *game, float x, float y, int w, int h);
    ~Sprite();
   
    virtual void update();
    virtual void draw();
};

#endif
