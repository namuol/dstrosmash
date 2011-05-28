/*
    ASTROSMASH!    
*/
#include <maxmod9.h>
#include <nds/system.h>

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

	videoSetModeSub(MODE_5_2D | DISPLAY_BG2_ACTIVE);
	REG_BG2CNT_SUB = BG_BMP16_256x256;
	REG_BG2PA_SUB = 256;
	REG_BG2PB_SUB = 0;
	REG_BG2PC_SUB = 0;
	REG_BG2PD_SUB = 256;
	REG_BG2Y_SUB = 0;
	REG_BG2X_SUB = 20;

	//Initially black
	REG_BLDCNT_SUB = BLEND_FADE_BLACK | BLEND_SRC_BG2;
	REG_BLDY_SUB = 0x1f;
    ulSetMainLcd(0);
     
    //Use bright pink as a transparent color
    ulSetTransparentColor(RGB15(31, 0, 31));

    Man::loadImages();
    Rock::loadImages();
    Spinner::loadImages();
    UFO::loadImages();
    UFOShot::loadImages();
    Missile::loadImages();

    SFX::init();

    //consoleDemoInit();
    while(1) {
        Game::inst()->init();
        Game::inst()->mainLoop();
    }

    //Program end - should never get there
    return 0;
}

