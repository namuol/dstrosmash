#include "Shot.h"
#include "UFO.h"
#include "Game.h"
#include "ufo.h"
#include "smash.h"

#include <ulib/ulib.h>

#include <list>
using namespace std;

UL_IMAGE* UFO::loaded_img = NULL;

UFO::UFO(Game *game)
: Sprite(game, 
         0, 
         RAND_RANGE(UFO_FRAME_HEIGHT/2,CEILING+(FLOOR/2)),
         UFO_FRAME_WIDTH/2,
         UFO_FRAME_HEIGHT/2)
{
    img = loaded_img;
    next_shot = UFO_SHOT_RATE;

    if(RAND(2)==1){
        vx = game->getRules()->ufo_speed;
        x = -UFO_FRAME_WIDTH/2;
    } else {
        vx = -game->getRules()->ufo_speed;
        x = RIGHT_WALL;
    }
}

UFO::~UFO() {
    game->ufos->remove(this);
}

void UFO::update() {
    if( (vx > 0 && LEFT(this) > RIGHT_WALL) ||
        (vx < 0 && RIGHT(this) < LEFT_WALL) ) {
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
    frame = ulAbs(UFO_FRAME_HEIGHT*((imgx/9)%UFO_FRAME_COUNT));

    if(next_shot <= 0)
    {
        shoot();
    }
    next_shot -= FRAME_LENGTH_MS;
    
    Sprite::update();
}

void UFO::draw() {
    ulSetImageTileSize(img, 0, frame,
                       UFO_FRAME_WIDTH, UFO_FRAME_HEIGHT);
    Sprite::draw();
}

void UFO::shoot() {
    game->ufo_shots->push_back(new UFOShot(game,this)); 
    next_shot = UFO_SHOT_RATE;
}

void UFO::kill(DeathType deathType) {
    switch( deathType ) {
        case OUT_OF_BOUNDS:
            delete this;
            break;
        case SHOT:
        case EXPLODED:
            game->explosions->push_back(new Explosion(game, x+w/2, y+h/2) );
            game->updateScore(UFO_SHOT_SCORE);

            //playGenericSound((void *)smash, (u32)smash_size);
            delete this;
            break;
        default:;
    }
}

void UFO::loadImages() {
    if( loaded_img==NULL ) 
        loaded_img=ulLoadImageFilePNG((const char*)ufo, (int)ufo_size, UL_IN_VRAM, UL_PF_PAL4);
}