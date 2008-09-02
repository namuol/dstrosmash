//Our main file
#include "main.h"
//Class definition
#include "Man.h"
#include "Shot.h"
#include "player.h"

UL_IMAGE * Man::img;

//Constructor
Man::Man(int x, int y) {
	this->img = ulLoadImageFilePNG((const char*)player, (int)player_size, UL_IN_VRAM, UL_PF_PAL2);

    this->x = x;
    this->y = y;
}

//Destructor
Man::~Man()	{
    ulDeleteImage(this->img);
}

void Man::update() {
    // NOTE: We assume that ulReadKeys(0) is called before each update. 
    if( ul_keys.held.left )
        this->x -= MAN_SPEED;
    else if( ul_keys.held.right )
        this->x += MAN_SPEED;

    this->x = ulMax(WALL_LEFT, this->x);
    this->x = ulMin(WALL_RIGHT, this->x);

    if( ul_keys.pressed.A )
        this->shoot();

    std::list<Shot *>::iterator i;
    std::list<Shot *> tmpShots;
    for(i = this->shots.begin(); i != this->shots.end(); ++i ) {
        tmpShots.push_back(*i);
    }

    for(i = tmpShots.begin(); i != tmpShots.end(); ++i ) {
        (*i)->update();
    }


}

void Man::draw() {
    ulDrawImageXY( this->img, this->x-7, this->y-8 );
    std::list<Shot *>::iterator i;
    for(i = this->shots.begin(); i != this->shots.end(); ++i ) {
        (*i)->draw();
    }
}

void Man::shoot() {
    // Fire a shot!
    if(this->shots.size() < MAX_SHOTS)
        this->shots.push_back( new Shot(this) );
}
