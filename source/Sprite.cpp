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

Sprite::Sprite()
{
    id = 0;
    imgx = 0;
    imgy = 0;
    img = 0;
    w = 0;
    h = 0;
    x = 0;
    y = 0;

}

Sprite::Sprite(const Sprite& other)
:
id(0),
imgx(other.imgx), imgy(other.imgy),
img(other.img),
w(other.w), h(other.h),
x(other.x), y(other.y)
{
}

//Constructor
Sprite::Sprite(UL_IMAGE *img, float x, float y) {
    this->img = img;
    this->x = x;
    this->y = y;
    w = img->sizeX/2;
    h = img->sizeY/2;
}


Sprite::Sprite(float x, float y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

//Destructor
Sprite::~Sprite() {
}

void Sprite::init(int id, UL_IMAGE *img, float x, float y)
{
    this->id = id;
    this->imgx = x;
    this->imgy = y;
    this->img = img;
    w = img->sizeX/2;
    h = img->sizeY/2;
    this->x = x;
    this->y = y;
    ++game->sprite_count;

}

void Sprite::init(int id, float x, float y, int w, int h)
{
    this->id = id;
    this->imgx = x;
    this->imgy = y;
    this->img = NULL;
    this->w = w;
    this->h = h;
    this->x = x;
    this->y = y;
    ++game->sprite_count;
}

void Sprite::deinit() {
    id = -1;
    --game->sprite_count;
}

void Sprite::update() {
    imgx = round(x)*2;
    imgy = round(y)*2;
}

void Sprite::draw() {
    if(img) ulDrawImageXY( img, imgx, imgy );
}
