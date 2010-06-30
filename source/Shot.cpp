//Our main file
#include "main.h"
//Class definition
#include "Shot.h"
#include "Game.h"

void Shot::init(int id) {
    Sprite::init(id,
                 round(Game::inst()->theMan->x)+MAN_WIDTH/2,
                 Game::inst()->theMan->y,
                 SHOT_WIDTH,
                 SHOT_HEIGHT);
}

void Shot::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( y > CEILING-SHOT_HEIGHT ) {
        y += SHOT_SPEED;
    } else {
        Game::inst()->shots.rem(id);
        Game::inst()->shots[id].deinit();
        return;
    }
    
    Sprite::update();
}

void Shot::draw() {
    ulDrawFillRect(imgx, imgy, imgx+SHOT_WIDTH*2, imgy+SHOT_HEIGHT*2, RGB15(31,31,31));
}
