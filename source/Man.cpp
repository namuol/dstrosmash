#include "Man.h"
#include "Sprite.h"
#include "Game.h"
#include "Rock.h"
#include "Explosion.h"

#include "math.h"
#include <ulib/ulib.h>
#include <list>
using namespace std;

#include "Shot.h"

#include "man.h"

//Constructor
Man::Man(Game *game, float x, float y) 
// Call superclass Sprite constructor. 
: Sprite( game, 
        ulLoadImageFilePNG((const char*)man,(int)man_size,UL_IN_VRAM,UL_PF_PAL4),
        x, y ) {
}

//Destructor
Man::~Man()	{
    ulDeleteImage(img);
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

    list<Rock *>::iterator r;
    list<Rock *> tmpRocks( *game->rocks ); 
    for(r = tmpRocks.begin(); r != tmpRocks.end(); ++r ) {
        if(COLTEST(this, (*r)) ) {
            game->death();
            return;
        }
    }

    list<UFOShot *>::iterator us;
    list<UFOShot *> tmpUFOShots( *game->ufo_shots ); 
    for(us = tmpUFOShots.begin(); us != tmpUFOShots.end(); ++us ) {
        if(COLTEST(this, (*us)) ) {
            game->death();
            return;
        }
    }

    list<Explosion *>::iterator e;
    list<Explosion *> tmpExplosions( *game->explosions ); 
    for(e = tmpExplosions.begin(); e != tmpExplosions.end(); ++e ) {
        if(COLTEST(this, (*e)) ) {
            game->death();
            return;
        }
    }

    Sprite::update();
}

void Man::shoot() {
    // Fire a shot!
    if(game->shots->size() < MAX_SHOTS) {
        game->shots->push_back( new Shot(game) );
    }
}
