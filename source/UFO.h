#ifndef _UFO_H
#define _UFO_H

#include "Sprite.h"
#define UFO_FRAME_COUNT 3 // The number of frames in the animation.
#define UFO_FRAME_HEIGHT 16
#define UFO_FRAME_WIDTH 32

#define UFO_SHOT_RATE 400
#define UFO_SHOT_SPEED 1.8

#define UFO_BEEP_MS 100
#define UFO_DRONE_MS 300

class UFO : public Sprite {
private:
    unsigned int frame;
    int next_shot;
    int next_beep;
    bool beeping;

public:
    float vx;
    static UL_IMAGE* loaded_img;
    static void loadImages();

    UFO(Game *game);
    ~UFO();
   
    void update();
    void draw();
    void shoot();
    void kill(DeathType deathType);
};

#endif
