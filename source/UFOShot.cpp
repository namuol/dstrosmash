#include "util.h"

#include <list>
using namespace std;

#include "math.h"
#include "Game.h"
#include "ufo_shot.h"

UL_IMAGE *UFOShot::image = NULL;

void UFOShot::init(int id, const UFO* parent) 
{
    Sprite::init(id,
           image, 
           parent->x+parent->w/4, 
           parent->y+parent->h/4);

    vy = 1.0;
    vx = ((x-game->theMan->x)*vy)/(y-game->theMan->y);
    
    float s = sqrt(vx*vx + vy*vy);
    vx /= s;
    vy /= s;
    vx *= game->speed_scale*UFO_SHOT_SPEED;
    vy *= game->speed_scale*UFO_SHOT_SPEED;
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

    for(int i=0; i<game->shots.capacity(); ++i) {
        if(game->shots.active(i) && 
           COLTEST(this, &game->shots[i]) ) {
            game->shots.rem(i);
            game->shots[i].deinit();
            kill(SHOT);
            return;
        }
    }


    for(int i=0; i<game->explosions.capacity(); ++i) {
        if(game->explosions.active(i) &&
            COLTEST(this, &(game->explosions[i])) ) {
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
            game->ufo_shots.rem(id);
            deinit();
            break;
        case SHOT:
        case EXPLODED:
            int eid;
            eid = game->explosions.add();
            if(eid >= 0) {
                game->explosions[eid].init(eid, x+w/2, y+h/2);
            }
            game->ufo_shots.rem(id);
            deinit();
            break;
        default:;
    }
}

void UFOShot::loadImages() {
    if ( image == NULL )
        image = ulLoadImageFilePNG((const char*)ufo_shot, (int)ufo_shot_size, UL_IN_VRAM, UL_PF_PAL4);
}
