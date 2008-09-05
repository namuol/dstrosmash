#include "Man.h"
#include "Game.h"
#include "math.h"

#include "Rock.h"
#include "Shot.h"

UL_IMAGE * Man::img = NULL;

//Constructor
Man::Man(Game *game, float x, float y) {
    if( img == NULL )
        img = ulLoadImageFilePNG((const char*)man, (int)man_size, UL_IN_VRAM, UL_PF_PAL4);
    this->game = game;
    this->x = x;
    this->y = y;
}

//Destructor
Man::~Man()	{
}

void Man::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( ul_keys.held.left )
        x -= MAN_SPEED;
    else if( ul_keys.held.right )
        x += MAN_SPEED;

    x = ulMin(WALL_LEFT, x);
    x = ulMax(WALL_RIGHT, x);

    if( ul_keys.pressed.A )
        shoot();
}

void Man::draw() {
    ulDrawImageXY( Man::img, round(x)*2-7, round(y)*2-8 );
}

void Man::shoot() {
    // Fire a shot!
    if(game->shots.size() < MAX_SHOTS)
        game->shots.push_back( new Shot(game) );
}
