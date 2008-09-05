//Our main file
#include "util.h"
#include "math.h"
//Class definition
#include "Game.h"
#include "Rock.h"
#include "lg1.h"

UL_IMAGE *Rock::images[NUM_ROCK_IMAGES];
const int Rock::ROCK_COLORS[] = {
                            Game::INTV_PALETTE[1],
                            Game::INTV_PALETTE[2],
                            Game::INTV_PALETTE[3],
                            Game::INTV_PALETTE[4],
                            Game::INTV_PALETTE[5],
                            Game::INTV_PALETTE[6] };

//Constructor
Rock::Rock(Game *game, Rock *parent, int num) {
    if ( images[0] == NULL ) {
        images[0] = ulLoadImageFilePNG((const char*)sm1, (int)sm1_size, UL_IN_VRAM, UL_PF_PAL4);
        images[1] = ulLoadImageFilePNG((const char*)sm2, (int)sm2_size, UL_IN_VRAM, UL_PF_PAL4);
        images[2] = ulLoadImageFilePNG((const char*)sm3, (int)sm3_size, UL_IN_VRAM, UL_PF_PAL4);
        images[3] = ulLoadImageFilePNG((const char*)lg1, (int)lg1_size, UL_IN_VRAM, UL_PF_PAL4);
        images[4] = ulLoadImageFilePNG((const char*)lg2, (int)lg2_size, UL_IN_VRAM, UL_PF_PAL4);
        images[5] = ulLoadImageFilePNG((const char*)lg3, (int)lg3_size, UL_IN_VRAM, UL_PF_PAL4);
    }
    color = Game::INTV_PALETTE[RAND(6)];
    this->game = game;
    img = images[RAND(NUM_ROCK_IMAGES)];

    vx = ulMax(MIN_ROCK_XSPEED, 0.01 * RAND(100*MAX_ROCK_XSPEED));
    vy = ulMax(MIN_ROCK_XSPEED, 0.01 * RAND(100*MAX_ROCK_YSPEED));
    vx *= RAND(2) ? -1 : 1; // Reverse the direction 1/2 the time.
    x = RAND(WALL_RIGHT);
    y = 0;

    if(parent != NULL) {
    
    } else {

    }
}

//Destructor
Rock::~Rock() {
    game->rocks.remove(this);
}

void Rock::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( y > FLOOR ) {
        delete this;
    }

    x += vx;
    y += vy;
}

void Rock::draw() {
    ulSetImageTint(Rock::img, color);
    ulDrawImageXY( Rock::img, round(x)*2, round(y)*2);
}

void Rock::kill(RockDeathType deathType) {

    switch( deathType ) {
        case LAND:
            break;

        case SHOT:
            break;

        case OUT_OF_BOUNDS:
            break;

        default:
            ;
    }
}
