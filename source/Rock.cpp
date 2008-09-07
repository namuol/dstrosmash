//Our main file
#include "util.h"

#include <list>
using namespace std;

#include "math.h"
//Class definition
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"
#include "Rock.h"
#include "lg1.h"
#include "smash.h"

// NOTE: The first half of the rock images MUST be small rocks.
UL_IMAGE *Rock::images[NUM_ROCK_IMAGES];

const int Rock::ROCK_COLORS[] = {
    LIGHT_BLUE,
    OFF_WHITE,
    YELLOW,
    ORANGE,
    PINK,
    GREEN
};

// Constructor
Rock::Rock(Game *game, Rock *parent, int num, int rock_num) 
: Sprite(game,
         images[rock_num], 
         parent ? parent->x + (parent->img->sizeX/4) - images[rock_num]->sizeX/4 : RAND(RIGHT_WALL), 
         parent ? parent->y : 0 )
{
    if ( images[0] == NULL ) {
        images[0] = ulLoadImageFilePNG((const char*)sm1, (int)sm1_size, UL_IN_VRAM, UL_PF_PAL4);
        images[1] = ulLoadImageFilePNG((const char*)sm2, (int)sm2_size, UL_IN_VRAM, UL_PF_PAL4);
        images[2] = ulLoadImageFilePNG((const char*)sm3, (int)sm3_size, UL_IN_VRAM, UL_PF_PAL4);
        images[3] = ulLoadImageFilePNG((const char*)lg1, (int)lg1_size, UL_IN_VRAM, UL_PF_PAL4);
        images[4] = ulLoadImageFilePNG((const char*)lg2, (int)lg2_size, UL_IN_VRAM, UL_PF_PAL4);
        images[5] = ulLoadImageFilePNG((const char*)lg3, (int)lg3_size, UL_IN_VRAM, UL_PF_PAL4);
    }
    this->rock_num = rock_num;
    is_big = rock_num >= 3;

    if( parent != NULL ) {
        if( num == 1 ) {
            //x -= img->sizeX/8;
            vx = -ROCK_SPLIT_SPEED + parent->vx;
        } else {
            //x += img->sizeX/8;
            vx = ROCK_SPLIT_SPEED + parent->vx;
        }
        vy = parent->vy;
        color = parent->color;
    } else {
        color = ROCK_COLORS[RAND(6)];

        vx = ulMax(MIN_ROCK_XSPEED, (float)rand()/RAND_MAX * MAX_ROCK_XSPEED);
        vy = ulMax(MIN_ROCK_YSPEED, (float)rand()/RAND_MAX * MAX_ROCK_YSPEED);
        vx *= RAND(2) ? -1 : 1; // Reverse the direction 1/2 the time.
    }
}

//Destructor
Rock::~Rock() {
    game->rocks.remove(this);
}

void Rock::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( BOTTOM(this) > FLOOR ) {
        kill(LAND);
        return;
    }

    list<Shot *>::iterator s;
    list<Shot *> tmpShots( game->shots ); 
    for(s = tmpShots.begin(); s != tmpShots.end(); ++s ) {
        if(COLTEST(this, (*s)) ) {
            delete (*s);
            kill(SHOT);
            return;
        }
    }

    list<Explosion *>::iterator e;
    list<Explosion *> tmpExplosions( game->explosions ); 
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

void Rock::draw() {
    ulSetImageTint(Rock::img, color);
    Sprite::draw();
}

void Rock::kill(RockDeathType deathType) {
    int rockNum;
    switch( deathType ) {
        case LAND:
            if( is_big )
                game->updateScore(BIG_ROCK_LAND_SCORE);
            else
                game->updateScore(SMALL_ROCK_LAND_SCORE);
        case OUT_OF_BOUNDS:
            delete this;
            break;

        case SHOT:
            if( is_big && (float)rand()/RAND_MAX < ROCK_SPLIT_PROBABILITY ){
                rockNum = RAND(NUM_ROCK_IMAGES/2);
                game->rocks.push_back(new Rock(game, this, 0, rockNum) );
                game->rocks.push_back(new Rock(game, this, 1, rockNum) );
            } else {
        case EXPLODED: // Yes, you can put case GOTOs inside conditional blocks. 
                game->explosions.push_back(new Explosion(game, x, y) );
            }
            if( is_big )
                game->updateScore(BIG_ROCK_SHOT_SCORE);
            else
                game->updateScore(SMALL_ROCK_SHOT_SCORE);

            playGenericSound((void *)smash, (u32)smash_size);
            delete this;
            break;

        default:
            ;
    }
}
