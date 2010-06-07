#include "Shot.h"
#include "Spinner.h"
#include "Game.h"
#include "spinner.h"
#include "smash.h"

#include <ulib/ulib.h>

#include <list>
using namespace std;

UL_IMAGE *Spinner::loaded_img = NULL;

Spinner::Spinner(Game *game)
: Sprite(game, 
         RAND(RIGHT_WALL), 
         0,
         SPINNER_FRAME_WIDTH/2,
         SPINNER_FRAME_HEIGHT/2 )
{
    if( loaded_img==NULL ) 
        loaded_img=ulLoadImageFilePNG((const char*)spinner, (int)spinner_size, UL_IN_VRAM, UL_PF_PAL4);
    img = loaded_img;

    vx = ulMax(MIN_SPINNER_XSPEED, (float)rand()/RAND_MAX * MAX_SPINNER_XSPEED);
    vy = ulMax(MIN_SPINNER_YSPEED, (float)rand()/RAND_MAX * MAX_SPINNER_YSPEED);
    vx *= RAND(2) ? -1 : 1; // Reverse the direction 1/2 the time.
    flipped = vx < 0;
}

Spinner::~Spinner() {
    game->spinners->remove(this);
}

void Spinner::update() {
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

void Spinner::draw() {
    ulSetImageTileSize(img, 0, SPINNER_FRAME_HEIGHT*((imgy/9)%SPINNER_FRAME_COUNT), SPINNER_FRAME_WIDTH, SPINNER_FRAME_HEIGHT); 
    ulMirrorImageH(img, flipped);
    Sprite::draw();
}

void Spinner::kill(DeathType deathType) {
    switch( deathType ) {
        case LAND:
            if( is_big )
                game->updateScore(BIG_SPINNER_LAND_SCORE);
            else
                game->updateScore(SMALL_SPINNER_LAND_SCORE);
            game->death(); // death() will call delete on it for us!
            break;
        case OUT_OF_BOUNDS:
            delete this;
            break;

        case SHOT:
        case EXPLODED:
            game->explosions->push_back(new Explosion(game, x, y) );
            if( is_big )
                game->updateScore(BIG_SPINNER_SHOT_SCORE);
            else
                game->updateScore(SMALL_SPINNER_SHOT_SCORE);

            //playGenericSound((void *)smash, (u32)smash_size);
            delete this;
            break;

        default:
            ;
    }
}
