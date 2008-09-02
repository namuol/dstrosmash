#ifndef _ROCK_H
#define _ROCK_H
#include <ulib/ulib.h>

enum RockDeathType { 
    LAND, // When the rock hits the ground
    SHOT, // When the rock is shot
    OUT_OF_BOUNDS // When the rock goes off the screen (doesn't affect the score)
};

class Rock {
private:
	int x, y;

public:
   Rock(Rock *parent, int num);
   ~Rock();
   
   void update();
   void draw();
   void kill(RockDeathType deathType);
};

#endif
