#include "Man.h"
#include "Sprite.h"
#include "Game.h"
#include "Rock.h"
#include "Explosion.h"

#include "math.h"
#include <ulib/ulib.h>
#include <list>
#include "sfx.h"

using namespace std;

#include "Shot.h"

#include "man.h"

//Constructor
Man::Man(float x, float y) 
: Sprite(ulLoadImageFilePNG((const char*)man,(int)man_size,UL_IN_VRAM,UL_PF_PAL4),
        x, y ) {
    next_shot = 400;
    auto_fire = false;
}

//Destructor
Man::~Man()	{
    //ulDeleteImg(img);
}

void Man::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( ul_keys.held.left )
        x -= MAN_SPEED;
    else if( ul_keys.held.right )
        x += MAN_SPEED;

    if(x < LEFT_WALL)
        x = LEFT_WALL;
    else if(RIGHT(this) > RIGHT_WALL)
        x = RIGHT_WALL - w + 1;

    if( ul_keys.pressed.A || ul_keys.pressed.B )
        shoot();

    if( ul_keys.pressed.X || ul_keys.pressed.Y )
        hyper();

    for(int i=0; i<game->rocks.capacity(); ++i) {
        if(game->rocks.active(i) &&
            COLTEST(this, &(game->rocks[i])) ) {
            game->death();
            return;
        }
    }

    for(int i=0; i<game->ufo_shots.capacity(); ++i) {
        if(game->ufo_shots.active(i) &&
            COLTEST(this, &(game->ufo_shots[i])) ) {
            game->death();
            return;
        }
    }

    for(int i=0; i<game->missiles.capacity(); ++i) {
        if(game->missiles.active(i) &&
            COLTEST(this, &(game->missiles[i])) ) {
            game->death();
            return;
        }
    }

    for(int i=0; i<game->explosions.capacity(); ++i) {
        if(game->explosions.active(i) &&
            COLTEST(this, &(game->explosions[i])) ) {
            game->death();
            return;
        }
    }
    
    if(auto_fire) {
        if(next_shot <= 0) shoot();
        next_shot -= FRAME_LENGTH_MS;
    }

    Sprite::update();
}

void Man::shoot() {
    // Fire a shot!
    if(game->shots.size() < MAX_SHOTS) {
        int id = game->shots.add();
        game->shots[id].init(id);
    }
    next_shot = 275;
}

void Man::hyper() {
    // Teleport to a random position
    SFX::hyper();
    x = RAND(RIGHT_WALL - w + 1);
}
