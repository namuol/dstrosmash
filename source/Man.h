#ifndef _MAN_H
#define _MAN_H
#include <ulib/ulib.h>
#include <list>
#include "Shot.h"

#define MAN_SPEED 3
#define MAX_SHOTS 2

class Man {
private:
	static UL_IMAGE *img;

public:
    int x, y;
    std::list<Shot *> shots;
    Man(int x, int y);
    ~Man();
   
    void update();
    void draw();
    void shoot();
};

#endif
