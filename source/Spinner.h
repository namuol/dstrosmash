#ifndef _SPINNER_H
#define _SPINNER_H

#include "Sprite.h"
#define SPINNER_FRAME_COUNT 4 // The number of frames in the animation.
#define SPINNER_FRAME_HEIGHT 16
#define SPINNER_FRAME_WIDTH 16

#define MIN_SPINNER_YSPEED 0.5
#define MAX_SPINNER_YSPEED 1
#define MIN_SPINNER_XSPEED 0
#define MAX_SPINNER_XSPEED 0.25

class Spinner : public Sprite {
private:
    bool is_big;
    bool flipped;

public:
    static UL_IMAGE* loaded_img_big;
    float vx, vy;
    static UL_IMAGE* loaded_img;
    static void loadImages();

    Spinner(Game *game);
    ~Spinner();
   
    void update();
    void draw();
    void kill(DeathType deathType);
};

#endif
