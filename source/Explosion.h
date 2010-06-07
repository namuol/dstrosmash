#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "Sprite.h"
#define EXPLOSION_FRAME_LENGTH 3 // The number of frames an individual animation frame takes
#define EXPLOSION_FRAME_COUNT 7 // The number of frames in the animation.
#define EXPLOSION_FRAME_HEIGHT 16
#define EXPLOSION_FRAME_WIDTH 16

class Explosion : public Sprite {
private:
    static UL_IMAGE *loaded_img;
    int frame_count;

public:
    Explosion(Game *game, float x, float y);
    ~Explosion();

    void update();
    void draw();
};

#endif
