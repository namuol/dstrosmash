/*
 * This is the base sprite class to be used by any other moving items on
 *  the screen.
 * 
 * Two floats (x, y) represent the top-left corner of the sprite on the game screen.
 * The coordinate is doubled to achieve proper pixely movement, but the sprite's image
 * is not doubled, to save speed.
 */

#include "Game.h"
#include "math.h"

//Constructor
Sprite::Sprite(Game *game, float x, float y) {
    this->game = game;
    this->x = x;
    this->y = y;
}

//Destructor
Sprite::~Sprite() {
}

void Sprite::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    imgx = round(x)*2;
    imgy = round(y)*2;
}

void Sprite::draw() {
    ulDrawImageXY( img, imgx, imgy );
}
