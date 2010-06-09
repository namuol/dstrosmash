#include "Shot.h"
#include "Spinner.h"
#include "Game.h"
#include "spinner.h"
#include "spinner_big.h"
#include "smash.h"

#include <ulib/ulib.h>

#include <list>
using namespace std;

UL_IMAGE *Spinner::loaded_img = NULL;
UL_IMAGE *Spinner::loaded_img_big = NULL;

Spinner::Spinner(Game *game)
: Sprite(game, 
         RAND(RIGHT_WALL), 
         0,0,0)
{
    is_big = RAND(3)==0;
    if(is_big)
        img = loaded_img_big;
    else
        img = loaded_img;

    if(is_big) {
       w = SPINNER_FRAME_WIDTH;
       h = SPINNER_FRAME_HEIGHT;
    }
    else
    {
       w = SPINNER_FRAME_WIDTH/2;
       h = SPINNER_FRAME_HEIGHT/2;
    }

    y = -h;

    float speed_scale = sqrt((float)game->multiplyer/(float)MAX_MULTIPLYER);
    vx = ulMax(MIN_SPINNER_XSPEED/(is_big?2:1), (float)rand()/RAND_MAX * MAX_SPINNER_XSPEED)*speed_scale;
    vy = ulMax(MIN_SPINNER_YSPEED/(is_big?2:1), (float)rand()/RAND_MAX * MAX_SPINNER_YSPEED)*speed_scale;
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
    ulSetImageTileSize(img, 0, ulAbs((SPINNER_FRAME_HEIGHT*(is_big?2:1))*(((imgy+2*h)/9)%SPINNER_FRAME_COUNT)),
                       (SPINNER_FRAME_WIDTH*(is_big?2:1)), (SPINNER_FRAME_HEIGHT*(is_big?2:1))); 
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
            game->explosions->push_back(new Explosion(game, x+w/2, y+h/2) );
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

void Spinner::loadImages() {
    if( loaded_img==NULL ) 
        loaded_img=ulLoadImageFilePNG((const char*)spinner, (int)spinner_size, UL_IN_VRAM, UL_PF_PAL4);
    if( loaded_img_big==NULL ) 
        loaded_img_big=ulLoadImageFilePNG((const char*)spinner_big, (int)spinner_big_size, UL_IN_VRAM, UL_PF_PAL4);
}
