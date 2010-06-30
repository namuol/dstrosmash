/*
    ASTROSMASH!    
*/
#include <maxmod9.h>

#include "main.h"

#include "soundbank.h"
#include "soundbank_bin.h"

#include "intv.h"

#include "util.h"

#include "sfx.h"

#include "Game.h"
#include "time.h"

int main() {
    UL_FONT* font;

    srand(time(NULL));

    //Initialize uLibrary
    ulInit(UL_INIT_ALL);
    //Initialize the graphical part
    ulInitGfx();
    //Initialize the text part
    ulInitText();
    
    font = ulLoadFontFile((const char*)intv, (int)intv_size);
    ulSetFont(font);

    ulSetMainLcd(0);
     
    //Use bright pink as a transparent color
    ulSetTransparentColor(RGB15(31, 0, 31));

    Rock::loadImages();
    Spinner::loadImages();
    UFO::loadImages();
    UFOShot::loadImages();
    Missile::loadImages();

    SFX::init();

    while(1) {
        Game::inst()->init();
        Game::inst()->mainLoop();
    }

    //Program end - should never get there
    return 0;
}

