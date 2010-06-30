#include "Shot.h"
#include "Missile.h"
#include "Game.h"
#include "missile.h"
#include "sfx.h"

#include <ulib/ulib.h>

#include <list>
using namespace std;

UL_IMAGE *Missile::loaded_img = NULL;

void Missile::init(int id)
{
    Sprite::init(id, 
           RAND(2)?RIGHT_WALL:0, 
           0,
           MISSILE_FRAME_WIDTH/2,
           MISSILE_FRAME_HEIGHT/2);
    next_blink = MISSILE_BLINK_ON_MS;
    next_beep = 0;
    frame = 1;
    blinking = true;
    speed = game->speed_scale*MISSILE_MAX_SPEED;

    vy = speed;
    vx = 0;
    img = loaded_img;
    update();

    offscreen = true;
}

void Missile::deinit() {
    SFX::missile_beep_stop();
}

void Missile::update() {
    float dy, dx, dr,
          vxi, vyi; // Ideal velocity
    if(!offscreen)
    {
        // NOTE: We assume that ulReadKeys(0) is called before each update. 
        if( BOTTOM(this) > FLOOR+1 ) {
            kill(LAND);
            return;
        }
        if( RIGHT(this) < LEFT_WALL ||
            LEFT(this) > RIGHT_WALL ) {
            kill(OUT_OF_BOUNDS);
            return;
        }

        for(int i=0; i<game->shots.capacity(); ++i) {
            if(game->shots.active(i) && 
               COLTEST(this, &(game->shots[i])) ) {
                game->shots.rem(i);
                game->shots[i].deinit();
                kill(SHOT);
                return;
            }
        }

        for(int i=0; i<game->rocks.capacity(); ++i) {
            if(game->rocks.active(i) &&
                COLTEST(this, &(game->rocks[i])) ) {
                game->rocks.rem(i);
                game->rocks[i].deinit();
                kill(COLLIDE);
                return;
            }
        }

        for(int i=0; i<game->ufos.capacity(); ++i) {
            if(game->ufos.active(i) &&
                COLTEST(this, &(game->ufos[i])) ) {
                game->ufos.rem(i);
                game->ufos[i].deinit();
                kill(COLLIDE);
                return;
            }
        }

        for(int i=0; i<game->ufo_shots.capacity(); ++i) {
            if(game->ufo_shots.active(i) &&
                COLTEST(this, &(game->ufo_shots[i])) ) {
                game->ufo_shots.rem(i);
                game->ufo_shots[i].deinit();
                kill(COLLIDE);
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
        
        dx = game->theMan->x + game->theMan->w*0.5 - x;
        dy = game->theMan->y + game->theMan->h*0.5 - y;
        dr = sqrt(dx*dx + dy*dy);

        vxi = (dx/dr)*speed;
        vyi = (dy/dr)*speed;
        
        vx += (vxi-vx)*MISSILE_MAX_ACC*game->speed_scale;
        vy += (vyi-vy)*MISSILE_MAX_ACC*game->speed_scale;

        dr = sqrt(vx*vx + vy*vy);
        vx = (vx/dr)*speed;
        vy = (vy/dr)*speed;

        x += vx;
        y += vy;
    }

    if(next_beep <= 0)
    {
        SFX::missile_beep();
        next_beep = MISSILE_BEEP_MS/game->speed_scale;
    } else if(next_beep <= 0.5*MISSILE_BEEP_MS/game->speed_scale)
    {
        offscreen = false;
        SFX::missile_beep_stop();
    }
    next_beep -= FRAME_LENGTH_MS;

    if(next_blink <= 0)
    {
        if(blinking)
            next_blink = MISSILE_BLINK_OFF_MS;
        else
            next_blink = MISSILE_BLINK_ON_MS;
        blinking = !blinking;
    }
    next_blink -= FRAME_LENGTH_MS;
    frame = MISSILE_FRAME_HEIGHT*(blinking?1:0);

    Sprite::update();
}

void Missile::draw() {
    if(offscreen) return;
    ulSetImageTileSize(img, 0, frame, MISSILE_FRAME_WIDTH, MISSILE_FRAME_HEIGHT); 
    Sprite::draw();
}

void Missile::kill(DeathType deathType) {
    int eid;
    switch( deathType ) {
        case LAND:
            eid = game->explosions.add();
            if(eid >= 0) {
                game->explosions[eid].init(eid, x+w/2, y+h/2);
            }
            game->updateScore(MISSILE_LAND_SCORE);
            game->missiles.rem(id);
            deinit();
            break;
        case OUT_OF_BOUNDS:
            game->missiles.rem(id);
            deinit();
            break;

        case SHOT:
        case EXPLODED:
            game->updateScore(MISSILE_SHOT_SCORE);
        case COLLIDE:
        default:
            eid = game->explosions.add();
            if(eid >= 0)
                game->explosions[eid].init(eid, x+w/2, y+h/2);
            game->missiles.rem(id);
            deinit();
            break;

    }
}

void Missile::loadImages() {
    if( loaded_img==NULL ) 
        loaded_img=ulLoadImageFilePNG((const char*)missile, (int)missile_size, UL_IN_VRAM, UL_PF_PAL4);
}
