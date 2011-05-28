#include "Shot.h"
#include "UFO.h"
#include "Game.h"
#include "ufo.h"

#include "sfx.h"

#include <ulib/ulib.h>

#include <list>
using namespace std;

UL_IMAGE* UFO::loaded_img = NULL;

void UFO::init(int id)
{
    Sprite::init(id, 
           0, 
           RAND_RANGE(UFO_FRAME_HEIGHT/2,CEILING+(FLOOR/2)),
           UFO_FRAME_WIDTH/2,
           UFO_FRAME_HEIGHT/2);
    img = loaded_img;
    next_shot = (int)((float)UFO_SHOT_RATE/game->speed_scale);
    next_beep = (int)((float)UFO_BEEP_MS/game->speed_scale);
    beeping = false;

    if(RAND(2)==1){
        vx = game->getRules()->ufo_speed*game->speed_scale;
        x = -UFO_FRAME_WIDTH/2;
    } else {
        vx = -game->getRules()->ufo_speed*game->speed_scale;
        x = RIGHT_WALL;
    }
    
    offscreen = true;

    SFX::ufo();
    update();
}

void UFO::deinit() {
    SFX::ufo_stop();
}

void UFO::update() {
    if(next_beep <= 0)
    {
        if(beeping)
        {
            SFX::ufo_beep_stop();
            next_beep = UFO_BEEP_MS/game->speed_scale;
        }
        else
        {
            offscreen = false;
            SFX::ufo_beep();
            next_beep = UFO_DRONE_MS/game->speed_scale;
        }
        beeping = !beeping;
    }
    next_beep -= FRAME_LENGTH_MS;

    if(offscreen) return;

    if( (vx > 0 && LEFT(this) > RIGHT_WALL) ||
        (vx < 0 && RIGHT(this) < LEFT_WALL) ) {
        kill(OUT_OF_BOUNDS);
        return;
    }

    for(unsigned int i=0; i<game->shots.capacity(); ++i) {
        if(game->shots.active(i) && 
           COLTEST(this, &(game->shots[i])) ) {
            game->shots.rem(i);
            game->shots[i].deinit();
            kill(SHOT);
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
    if(offscreen) return;
    ulSetImageTileSize(img, 0, frame,
                       UFO_FRAME_WIDTH, UFO_FRAME_HEIGHT);
    Sprite::draw();
}

void UFO::shoot() {
    int sid = game->ufo_shots.add();
    if(sid >= 0)
        game->ufo_shots[sid].init(sid, this);
    next_shot = (int)((float)UFO_SHOT_RATE/game->speed_scale);
}

void UFO::kill(DeathType deathType) {
    switch( deathType ) {
        case OUT_OF_BOUNDS:
            game->ufos.rem(id);
            deinit();
            break;
        case SHOT:
        case EXPLODED:
            int eid;
            eid = game->explosions.add();
            if(eid >= 0) {
                game->explosions[eid].init(eid, x+w/2, y+h/2);
            }

            game->updateScore(UFO_SHOT_SCORE);

            game->ufos.rem(id);
            deinit();
            break;
        default:;
    }
}

void UFO::loadImages() {
    if( loaded_img==NULL ) 
        loaded_img=ulLoadImageFilePNG((const char*)ufo, (int)ufo_size, UL_IN_VRAM, UL_PF_PAL4);
}
