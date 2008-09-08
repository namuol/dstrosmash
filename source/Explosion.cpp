#include "Explosion.h"
#include "Game.h"
#include "explosion.h"

#include <ulib/ulib.h>

UL_IMAGE *Explosion::loaded_img = NULL;

Explosion::Explosion(Game *game, float x, float y)
: Sprite(game, 
         x, y,
         EXPLOSION_FRAME_WIDTH/2,
         EXPLOSION_FRAME_HEIGHT/2 )
{
    if( loaded_img==NULL ) 
        loaded_img=ulLoadImageFilePNG((const char*)explosion, (int)explosion_size, UL_IN_VRAM, UL_PF_PAL4);
    img = loaded_img;
    frame_count = 0;
}

Explosion::~Explosion() {
    game->explosions->remove(this);
}

void Explosion::update() {
    ++frame_count;
    if( frame_count > EXPLOSION_FRAME_COUNT*EXPLOSION_FRAME_LENGTH ) {
        delete this;
        return;
    }

    Sprite::update();
}

void Explosion::draw() {
    ulSetImageTileSize(img, 0, EXPLOSION_FRAME_HEIGHT*(frame_count/EXPLOSION_FRAME_LENGTH), EXPLOSION_FRAME_WIDTH, EXPLOSION_FRAME_HEIGHT); 
    ulSetImageTint(img, RGB15(31,0,0));
    Sprite::draw();
}
