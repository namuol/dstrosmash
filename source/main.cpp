/*
    ASTROSMASH!    
*/
#include <maxmod9.h>

#include "main.h"

#include "soundbank.h"
#include "soundbank_bin.h"

#include "intv.h"

#include "util.h"

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

    Game game;
    Rock::loadImages();
    Spinner::loadImages();
    UFO::loadImages();
    UFOShot::loadImages();
    Missile::loadImages();

    // Set up maxmod (SFX)
    mmInitDefaultMem( (mm_addr)soundbank_bin );
    //mmSelectMode( MM_MODE_A );

    // Load some SFX:
    mmLoadEffect( SFX_DEATH1 );
    mmLoadEffect( SFX_DEATH2 );
    mmLoadEffect( SFX_DEATH3 );
    mmLoadEffect( SFX_DEATH4 );
    mmLoadEffect( SFX_DEATH5 );
    mmLoadEffect( SFX_DEATH6 );
    mmLoadEffect( SFX_DEATH7 );
    mmLoadEffect( SFX_FUTILITY1 );
    mmLoadEffect( SFX_FUTILITY2 );
    mmLoadEffect( SFX_FUTILITY3 );
    mmLoadEffect( SFX_FUTILITY4 );
    mmLoadEffect( SFX_FUTILITY5 );
    mmLoadEffect( SFX_HIT1 );
    mmLoadEffect( SFX_MISSILE );
    mmLoadEffect( SFX_SPINNER );
    mmLoadEffect( SFX_UFO );

    game.mainLoop();

    //Program end - should never get there
    return 0;
}

