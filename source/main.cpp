/*
    ASTROSMASH!	
*/

#include "main.h"
#include "Man.h"
#include "Shot.h"

void input() {
}

int main() {
	int totalTime = 1;						//To avoid a divide by zero the first time
   
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

	//Use bright pink as a transparent color
	ulSetTransparentColor(RGB15(31, 0, 31));
    Man *theMan = new Man(128, FLOOR);	
	while(1) {
	    //Initialize the timers to measure the framerate
	    TIMER1_CR = 0;
	    TIMER1_DATA = 0;
		TIMER1_CR = TIMER_DIV_64 | TIMER_ENABLE;

	    //Read keys
	    ulReadKeys(0);
	   
        theMan->update();

		ulStartDrawing2D();
		
        // First draw background

        // Then draw man
        theMan->draw();
         

		//Calculate the CPU usage based on the timer value and total time per frame
		int cpuTime = (TIMER1_DATA * 1000) / totalTime;
		ulPrintf_xy(0, 0, "CPU: %02i.%i%%", cpuTime / 10, cpuTime % 10);

		//End the drawing
		ulEndDrawing();
		
		//Wait the VBlank (synchronize at 60 fps)
		ulSyncFrame();
		totalTime = TIMER1_DATA;
	}

	//Program end - should never get there
	return 0;
}

