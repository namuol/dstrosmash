#include "Man.h"
#include "Sprite.h"
#include "Game.h"

#include "math.h"
#include <ulib/ulib.h>

#include "Shot.h"

#include "man.h"

//Constructor
Man::Man(Game *game, float x, float y) 
: Sprite( game, ulLoadImageFilePNG((const char*)man, (int)man_size, UL_IN_VRAM, UL_PF_PAL4), x, y ) // Call superclass Sprite constructor. 
{
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

    if( ul_keys.pressed.A )
        shoot();

    Sprite::update();
}

void Man::shoot() {
    // Fire a shot!
    if(game->shots.size() < MAX_SHOTS) {
        game->shots.push_back( new Shot(game) );
    }
}
