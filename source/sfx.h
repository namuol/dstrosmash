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

    static void update();

    static void futility();
    static void death();
    static void hit(); 
    static void missile_beep();
    static void missile_beep_cancel();

    static int missiles_playing;
    static void missile_start();
    static void missile_stop();
    
    static void spinner(float pitch = 1.0);
    static void spinner_cancel();
    static float spinner_min_pitch;

    static int spinners_playing;
    static void spinner_start();
    static void spinner_stop();

    static mm_sfxhand futility_h;
    static mm_sfxhand death_h;
    static mm_sfxhand hit_h;
    static mm_sfxhand missile_h;
    static mm_sfxhand spinner_h;

    static bool muted;
};
#endif
