//Our main file
#include "main.h"
//Class definition
#include "Shot.h"
#include "Game.h"

//Constructor
Shot::Shot(Game *game) 
: Sprite( game, round(game->theMan->x)+MAN_WIDTH/2, game->theMan->y, SHOT_WIDTH, SHOT_HEIGHT )
{
}

//Destructor
Shot::~Shot() {
    game->shots.remove(this);
}

void Shot::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( y > CEILING-SHOT_HEIGHT ) {
        y += SHOT_SPEED;
    } else {
        delete this;
        return;
    }
    
    Sprite::update();
}

void Shot::draw() {
    ulDrawFillRect(imgx, imgy, imgx+SHOT_WIDTH*2, imgy+SHOT_HEIGHT*2, RGB15(31,31,31));
}
