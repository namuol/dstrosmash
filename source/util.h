#ifndef _UTIL_H
#define _UTIL_H

#include <cmath>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define RAND(max) (rand()%(max))
#define RAND_NEGATIVE() (RAND(2)?-1:1)
#define RAND_RANGE(min,max) ((min)+rand()%((max)-(min)+1))
#define SHAKE(amt) ulMoveScreenView(RAND_NEGATIVE()*round(((float)rand()/RAND_MAX)*amt), RAND_NEGATIVE()*round(((float)rand()/RAND_MAX)*amt))

#endif
