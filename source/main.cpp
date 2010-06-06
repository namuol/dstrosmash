/*
    ASTROSMASH!	
*/

#include "main.h"
#include "util.h"

#include "Game.h"
#include "time.h"

int main() {

    srand(time(NULL));

	//Initialize uLibrary
	ulInit(UL_INIT_ALL);
	//Initialize the graphical part
	ulInitGfx();
	//Initialize the text part
	ulInitText();

	ulSetMainLcd(0);
	//Show some splash screens!
	//ulShowSplashScreen(1);
	//ulShowSplashScreen(2);
    //setGenericSound(	22050,	/* sample rate */
	//					127,	/* volume */
	//					64,		/* panning */
	//					0 );	/* sound format*/

 
	//Use bright pink as a transparent color
	ulSetTransparentColor(RGB15(31, 0, 31));

    Game game;
    game.mainLoop();

	//Program end - should never get there
	return 0;
}

