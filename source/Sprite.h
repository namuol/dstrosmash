#ifndef _SPRITE_H
#define _SPRITE_H

#include <ulib/ulib.h>

class Game;

class Sprite {
protected:
    int imgx, imgy; // The actual on-screen coordinates of the sprite.
	UL_IMAGE *img;
    Game *game;

public:
    float x, y;
    Sprite(Game *game, float x, float y);
    ~Sprite();
   
    virtual void update();
    virtual void draw();
};

#endif
