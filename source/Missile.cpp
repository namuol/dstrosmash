#include "Shot.h"
#include "Missile.h"
#include "Game.h"
#include "missile.h"
#include "sfx.h"

#include <ulib/ulib.h>

#include <list>
using namespace std;

UL_IMAGE *Missile::loaded_img = NULL;

Missile::Missile(Game *game)
: Sprite(game, 
         RAND(2)?RIGHT_WALL:0, 
         0,
         MISSILE_FRAME_WIDTH/2,
         MISSILE_FRAME_HEIGHT/2),
  next_blink(MISSILE_BLINK_ON_MS),
  next_beep(0),
  frame(1),
  blinking(true),
  speed(game->speed_scale*MISSILE_MAX_SPEED)
{
    //SFX::missile_start();
    vy = speed;
    vx = 0;
    img = loaded_img;
}

Missile::~Missile() {
    //SFX::missile_stop();
    SFX::missile_beep_stop();
    game->missiles->remove(this);
}

void Missile::update() {
    float dy, dx, dr,
          vxi, vyi; // Ideal velocity

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

    list<Shot *>::iterator s;
    list<Shot *> tmpShots( *game->shots ); 
    for(s = tmpShots.begin(); s != tmpShots.end(); ++s ) {
        if(COLTEST(this, (*s)) ) {
            delete (*s);
            kill(SHOT);
            return;
        }
    }

    list<Rock *>::iterator r;
    list<Rock *> tmpRocks( *game->rocks ); 
    for(r = tmpRocks.begin(); r != tmpRocks.end(); ++r ) {
        if(COLTEST(this, (*r)) ) {
            delete (*r);
            kill(COLLIDE);
            return;
        }
    }

    list<UFO *>::iterator u;
    list<UFO *> tmpUFOs( *game->ufos ); 
    for(u = tmpUFOs.begin(); u != tmpUFOs.end(); ++u ) {
        if(COLTEST(this, (*u)) ) {
            delete (*u);
            kill(COLLIDE);
            return;
        }
    }

    list<UFOShot *>::iterator us;
    list<UFOShot *> tmpUFOShots( *game->ufo_shots ); 
    for(us = tmpUFOShots.begin(); us != tmpUFOShots.end(); ++us ) {
        if(COLTEST(this, (*us)) ) {
            delete (*us);
            kill(COLLIDE);
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

    if(next_beep <= 0)
    {
        SFX::missile_beep();
        next_beep = MISSILE_BEEP_MS/game->speed_scale;
    } else if(next_beep <= 0.5*MISSILE_BEEP_MS/game->speed_scale)
    {
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
    ulSetImageTileSize(img, 0, frame, MISSILE_FRAME_WIDTH, MISSILE_FRAME_HEIGHT); 
    Sprite::draw();
}

void Missile::kill(DeathType deathType) {
    switch( deathType ) {
        case LAND:
            game->explosions->push_back(new Explosion(game, x+w/2, y+h/2) );
            game->updateScore(MISSILE_LAND_SCORE);
            delete this;
            break;
        case OUT_OF_BOUNDS:
            delete this;
            break;

        case SHOT:
        case EXPLODED:
            game->updateScore(MISSILE_SHOT_SCORE);
        case COLLIDE:
        default:
            game->explosions->push_back(new Explosion(game, x+w/2, y+h/2) );
            delete this;
            break;

    }
}

void Missile::loadImages() {
    if( loaded_img==NULL ) 
        loaded_img=ulLoadImageFilePNG((const char*)missile, (int)missile_size, UL_IN_VRAM, UL_PF_PAL4);
}
