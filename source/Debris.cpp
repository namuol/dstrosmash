#include "Game.h"
#include "Debris.h"

void Debris::init(int id, float x, float y, float vx, float vy)
{
    this->vx = vx;
    this->vy = vy;
    Sprite::init(id,Rock::images[0],x,y);
    update();
}

void Debris::update() {
    x+=vx;
    y+=vy;
    Sprite::update();
}

void Debris::draw() {
    ulSetImageTint(img, WHITE);
    Sprite::draw();
}
