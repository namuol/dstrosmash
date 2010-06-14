#ifndef _SFX_H
#define _SFX_H
#include <maxmod9.h>

struct SFX {

    #define FUTILITY_FX_COUNT 5
    static const unsigned int FUTILITY[];

    #define DEATH_FX_COUNT 7
    static const unsigned int DEATH[];

    #define HIT_FX_COUNT 1
    static const unsigned int HIT[];

    static void futility();
    static void death();
    static void hit(); 

    static int missiles_playing;
    static void missile_start();
    static void missile_stop();

    static void stop_all();

    static mm_sfxhand futility_h;
    static mm_sfxhand death_h;
    static mm_sfxhand hit_h;

    static bool muted;
};
#endif
