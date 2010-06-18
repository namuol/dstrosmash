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
    static void missile_beep_stop();

    static void spinner(float pitch = 1.0);
    static void spinner_stop();
    static float spinner_min_pitch;

    static void ufo();
    static void ufo_stop();
    static void ufo_beep();
    static void ufo_beep_stop();

    static mm_sfxhand futility_h;
    static mm_sfxhand death_h;
    static mm_sfxhand hit_h;
    static mm_sfxhand missile_h;
    static mm_sfxhand spinner_h;
    static mm_sfxhand ufo_h;

    static mm_sound_effect futility_sf;
    static mm_sound_effect death_sf;
    static mm_sound_effect hit_sf;
    static mm_sound_effect missile_sf;
    static mm_sound_effect spinner_sf;
    static mm_sound_effect ufo_sf;

    static bool muted;
};
#endif
