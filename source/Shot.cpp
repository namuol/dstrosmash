//Our main file
#include "main.h"
//Class definition
#include "Shot.h"
#include "Game.h"

//Constructor
Shot::Shot(Game *game) {
    this->game = game;
    this->x = game->theMan->x;
    this->y = game->theMan->y;
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
    }
}

void Shot::draw() {
    ulDrawFillRect(this->x, this->y, this->x+SHOT_WIDTH, this->y+SHOT_HEIGHT, RGB15(31,31,31));
}
