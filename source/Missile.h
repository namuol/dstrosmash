#ifndef _MISSILE_H
#define _MISSILE_H

#include "Sprite.h"
#define MISSILE_FRAME_COUNT 2 // The number of frames in the animation.
#define MISSILE_FRAME_HEIGHT 10
#define MISSILE_FRAME_WIDTH 10

#define MISSILE_MAX_SPEED 2.5
#define MISSILE_MAX_ACC 0.3
#define MISSILE_BLINK_ON_MS 300
#define MISSILE_BLINK_OFF_MS 100

#define MISSILE_BEEP_MS 300

class Missile : public Sprite {
private:
    int next_blink;
    int next_beep;
    int frame;
    bool blinking;

public:
    float speed;
    float angle;
    float vx, vy;
    static UL_IMAGE* loaded_img;
    static void loadImages();

    Missile(Game *game);
    ~Missile();
   
    void update();
    void draw();
    void kill(DeathType deathType);
};

#endif
