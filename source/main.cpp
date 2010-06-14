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
    //Show some splash screens!
    //ulShowSplashScreen(1);
    //ulShowSplashScreen(2);
    //setGenericSound(    22050,    /* sample rate */
    //                    127,    /* volume */
    //                    64,        /* panning */
    //                    0 );    /* sound format*/

 
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

    mmLoad( MOD_MISSILE );
    mmSetModuleVolume( 0 );
    mmStart( MOD_MISSILE, MM_PLAY_LOOP );

    game.mainLoop();

    //Program end - should never get there
    return 0;
}

