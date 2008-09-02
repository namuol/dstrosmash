//Our main file
#include "main.h"
//Class definition
#include "Shot.h"
#include "Man.h"

//Constructor
Shot::Shot(Man *m) {
    this->m = m;
    this->x = m->x;
    this->y = m->y;
}

//Destructor
Shot::~Shot() {
    m->shots.remove(this);
}

void Shot::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( this->y > CEILING-SHOT_HEIGHT ) {
        this->y += SHOT_SPEED;
    } else {
        delete this;
    }
}

void Shot::draw() {
        ulDrawFillRect(this->x, this->y, this->x+SHOT_WIDTH, this->y+SHOT_HEIGHT, RGB15(31,31,31));
}
