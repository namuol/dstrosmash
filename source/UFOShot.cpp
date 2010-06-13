//Our main file
#include "util.h"

#include <list>
using namespace std;

#include "math.h"
//Class definition
#include "Game.h"
#include "ufo_shot.h"

UL_IMAGE *UFOShot::image = NULL;

UFOShot::UFOShot(Game* game, const UFO* parent) 
: Sprite(game,
         image, 
         parent->x+parent->w/4, 
         parent->y+parent->h/4)
{
    vy = 1.0;
    vx = ((x-game->theMan->x)*vy)/(y-game->theMan->y);
    
    float s = sqrt(vx*vx + vy*vy);
    vx /= s;
    vy /= s;
    vx *= game->speed_scale*UFO_SHOT_SPEED;
    vy *= game->speed_scale*UFO_SHOT_SPEED;
}

//Destructor
UFOShot::~UFOShot() {
    game->ufo_shots->remove(this);
}

void UFOShot::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 

    if( BOTTOM(this) > FLOOR ) {
        kill(LAND);
        return;
    }
    if( MIDX(this) < LEFT_WALL ||
        MIDX(this) > RIGHT_WALL ) {
        kill(OUT_OF_BOUNDS);
        return;
    }

    list<Shot *>::iterator s;
    list<Shot *> tmpShots( *game->shots ); 
    for(s = tmpShots.begin(); s != tmpShots.end(); ++s ) {
        if(COLTEST(this, (*s)) ) {
            delete (*s);
            kill(SHOT);
            return;
        }
    }

    list<Explosion *>::iterator e;
    list<Explosion *> tmpExplosions( *game->explosions ); 
    for(e = tmpExplosions.begin(); e != tmpExplosions.end(); ++e ) {
        if(COLTEST(this, (*e)) ) {
            kill(EXPLODED);
            return;
        }
    }

    x += vx;
    y += vy;
    
    Sprite::update();
}

void UFOShot::draw() {
    Sprite::draw();
}

void UFOShot::kill(DeathType deathType) {
    switch( deathType ) {
        case LAND:
            // TODO: Score? Need to do my research as the manual doesnt say anything.
            //game->shake_amt += ROCK_LAND_SHAKE;
        case OUT_OF_BOUNDS:
            delete this;
            break;
        case SHOT:
        case EXPLODED:
            game->explosions->push_back(new Explosion(game, x+w/2, y+h/2) );
            //playGenericSound((void *)smash, (u32)smash_size);
            delete this;
            break;
        default:;
    }
}

void UFOShot::loadImages() {
    if ( image == NULL )
        image = ulLoadImageFilePNG((const char*)ufo_shot, (int)ufo_shot_size, UL_IN_VRAM, UL_PF_PAL4);
}
