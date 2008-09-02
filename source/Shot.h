#ifndef _SHOT_H
#define _SHOT_H
#include <ulib/ulib.h>
#include <list>
class Man;
#define SHOT_SPEED -10
#define SHOT_WIDTH 4
#define SHOT_HEIGHT 16
class Shot {
private:
	int x, y;
    Man *m;

public:
   Shot(Man *m);
   ~Shot();
   
   void update();
   void draw();
};

#endif
