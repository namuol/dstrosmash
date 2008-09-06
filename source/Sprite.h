#ifndef _SPRITE_H
#define _SPRITE_H

#include <ulib/ulib.h>

#define BOTTOM(s) ((s)->y+(s)->h)
#define TOP(s) ((s)->y)
#define LEFT(s) ((s)->x)
#define RIGHT(s) ((s)->x+(s)->w)

// Who's ever heard of a safe *and* useful macro, anyway?
#define COLTEST(s1, s2) !(BOTTOM(s1) < TOP(s2) || TOP(s1) > BOTTOM(s2) || RIGHT(s1) < LEFT(s2) || LEFT(s1) > RIGHT(s2))

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
