//Our main file
#include "main.h"
//Class definition
#include "Rock.h"

//Constructor
Rock::Rock(Rock *parent, int num) {
    if(parent != NULL) {
    
    } else {

    }
}

//Destructor
Rock::~Rock() {
}

void Rock::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
}

void Rock::draw() {
}

void Rock::kill(RockDeathType deathType) {

    switch( deathType ) {
        case LAND:
            break;

        case SHOT:
            break;

        case OUT_OF_BOUNDS:
            break;

        default:
            ;
    }
}
