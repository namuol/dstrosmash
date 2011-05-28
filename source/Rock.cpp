#include "util.h"

#include <list>
using namespace std;

#include "math.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"
#include "Rock.h"
#include "lg1.h"

#include "sfx.h"

// NOTE: The first half of the rock images MUST be small rocks.
UL_IMAGE *Rock::images[NUM_ROCK_IMAGES];

static int X1_ROCK_COLORS[] = {
    6,
    ORANGE,
    LIGHT_BLUE,
    PINK,
    GREEN,
    OFF_WHITE,
    YELLOW
};

static int X2_ROCK_COLORS[] = {
    5,
    ORANGE,
    LIGHT_BLUE,
    PINK,
    OFF_WHITE,
    YELLOW
};

static int X3_ROCK_COLORS[] = {
    5,
    ORANGE,
    LIGHT_BLUE,
    GREEN,
    OFF_WHITE,
    YELLOW
};

static int X4_ROCK_COLORS[] = {
    4,
    ORANGE,
    OFF_WHITE,
    YELLOW,
    DARK_GREEN
};

static int X5_ROCK_COLORS[] = {
    3,
    GREEN,
    YELLOW,
    DARK_GREEN
};

static int *X6_ROCK_COLORS = X1_ROCK_COLORS;


// Each int is a series of color flags ORd 
//  together; the values are defined in Game.h
const int *Rock::ROCK_COLORS[] = {
    X1_ROCK_COLORS,
    X2_ROCK_COLORS,
    X3_ROCK_COLORS,
    X4_ROCK_COLORS,
    X5_ROCK_COLORS,
    X6_ROCK_COLORS
};


// Constructor
void Rock::init(int id, Rock *parent, int num, int rock_num) 
{
    Sprite::init(id,
                 images[rock_num], 
                 parent ? parent->x + (parent->img->sizeX/4) - images[rock_num]->sizeX/4 : RAND(RIGHT_WALL), 
                 parent ? parent->y : 0);

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
        y = -img->sizeY/2;
        color = ROCK_COLORS[game->multiplyer-1][RAND(ROCK_COLORS[game->multiplyer-1][0])+1];
        vx = ulMax(MIN_ROCK_XSPEED, (float)rand()/RAND_MAX * MAX_ROCK_XSPEED)*game->speed_scale;
        vy = ulMax(MIN_ROCK_YSPEED, (float)rand()/RAND_MAX * MAX_ROCK_YSPEED)*game->speed_scale;
        vx *= RAND(2) ? -1 : 1; // Reverse the direction 1/2 the time.
    }
    flipped = vx < 0;

    update();
}

void Rock::update() {
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

    for(unsigned int i=0; i<game->shots.capacity(); ++i) {
        if(game->shots.active(i) && 
           COLTEST(this, &game->shots[i]) ) {
            game->shots.rem(i);
            game->shots[i].deinit();
            kill(SHOT);
            return;
        }
    }

    for(unsigned int i=0; i<game->ufo_shots.capacity(); ++i) {
        if(game->ufo_shots.active(i) && 
           COLTEST(this, &game->ufo_shots[i]) ) {
            game->ufo_shots.rem(i);
            game->ufo_shots[i].deinit();
            kill(SHOT);
            return;
        }
    }


    for(unsigned int i=0; i<game->explosions.capacity(); ++i) {
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

void Rock::draw() {
    ulSetImageTint(Rock::img, color);
    ulMirrorImageH(img, flipped);
    Sprite::draw();
}

void Rock::kill(DeathType deathType) {
    int rockNum;
    switch( deathType ) {
        case LAND:
            if( is_big )
                game->updateScore(BIG_ROCK_LAND_SCORE);
            else
                game->updateScore(SMALL_ROCK_LAND_SCORE);
            game->shake_amt += ROCK_LAND_SHAKE;
        case OUT_OF_BOUNDS:
            game->rocks.rem(id);
            deinit();
            break;

        case SHOT:
            if( is_big && (float)rand()/RAND_MAX < ROCK_SPLIT_PROBABILITY ){
                int id1, id2;
                SFX::hit();
                rockNum = RAND(NUM_ROCK_IMAGES/2);
                id1 = game->rocks.add();
                id2 = game->rocks.add();
                game->rocks[id1].init(id1, this, 0, rockNum);
                game->rocks[id2].init(id2, this, 1, rockNum);

            } else {
        case EXPLODED: // Yes, you can put case GOTOs inside conditional blocks. 
                int id;
                id = game->explosions.add();
                if(id >= 0) {
                    game->explosions[id].init(id, x+w/2, y+h/2);
                }

            }
            if( is_big )
                game->updateScore(BIG_ROCK_SHOT_SCORE);
            else
                game->updateScore(SMALL_ROCK_SHOT_SCORE);

            game->rocks.rem(id);
            deinit();
            break;

        default:
            ;
    }
}

void Rock::loadImages() {
    if ( images[0] == NULL ) {
        images[0] = ulLoadImageFilePNG((const char*)sm1, (int)sm1_size, UL_IN_VRAM, UL_PF_PAL4);
        images[1] = ulLoadImageFilePNG((const char*)sm2, (int)sm2_size, UL_IN_VRAM, UL_PF_PAL4);
        images[2] = ulLoadImageFilePNG((const char*)sm3, (int)sm3_size, UL_IN_VRAM, UL_PF_PAL4);
        images[3] = ulLoadImageFilePNG((const char*)lg1, (int)lg1_size, UL_IN_VRAM, UL_PF_PAL4);
        images[4] = ulLoadImageFilePNG((const char*)lg2, (int)lg2_size, UL_IN_VRAM, UL_PF_PAL4);
        images[5] = ulLoadImageFilePNG((const char*)lg3, (int)lg3_size, UL_IN_VRAM, UL_PF_PAL4);
    }
}
