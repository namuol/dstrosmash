#include <nds/ndstypes.h>

#include "sfx.h"
#include "util.h"
#include "soundbank.h"
#include "soundbank_bin.h"

const unsigned int SFX::FUTILITY[] = {
    SFX_FUTILITY1,
    SFX_FUTILITY2, 
    SFX_FUTILITY3, 
    SFX_FUTILITY4, 
    SFX_FUTILITY5
};

const unsigned int SFX::DEATH[] = {
    SFX_DEATH1,
    SFX_DEATH2, 
    SFX_DEATH3, 
    SFX_DEATH4, 
    SFX_DEATH5,
    SFX_DEATH6,
    SFX_DEATH7
};

const unsigned int SFX::HIT[] = {
    SFX_HIT1
};

mm_sfxhand SFX::futility_h = 1;
mm_sfxhand SFX::death_h = 2;
mm_sfxhand SFX::hit_h = 3;
mm_sfxhand SFX::missile_h = 4;
mm_sfxhand SFX::spinner_h = 5; 
mm_sfxhand SFX::ufo_h = 6;
mm_sfxhand SFX::hyper_h = 7;

mm_sound_effect SFX::futility_sf = {
    {FUTILITY[RAND(FUTILITY_FX_COUNT)]},
    1024,
    futility_h,
    255,
    127
};

mm_sound_effect SFX::death_sf = {
    {DEATH[RAND(DEATH_FX_COUNT)]},
    1024,
    death_h,
    255,
    127
};

mm_sound_effect SFX::hit_sf = {
    {HIT[RAND(HIT_FX_COUNT)]},
    1024,
    hit_h,
    255,
    127
};

mm_sound_effect SFX::missile_sf = {
    {SFX_MISSILE},
    1024,
    missile_h,
    255,
    127
};

mm_sound_effect SFX::spinner_sf = {
    {SFX_SPINNER},
    1024,
    spinner_h,
    255,
    127
};

mm_sound_effect SFX::ufo_sf = {
    {SFX_UFO},
    1024,
    ufo_h,
    127,
    127
};

mm_sound_effect SFX::hyper_sf = {
    {SFX_HYPER},
    1024,
    hyper_h,
    127,
    127
};


float SFX::spinner_min_pitch = 2.0;
bool SFX::muted = false;

void SFX::init() {
    // Set up maxmod (SFX)
    mmInitDefaultMem( (mm_addr)soundbank_bin );
    //mmSelectMode( MM_MODE_A );

    // Load some SFX:
    mmLoadEffect( SFX_DEATH1 );
    mmLoadEffect( SFX_DEATH2 );
    mmLoadEffect( SFX_DEATH3 );
    mmLoadEffect( SFX_DEATH4 );
    mmLoadEffect( SFX_DEATH5 );
    mmLoadEffect( SFX_DEATH6 );
    mmLoadEffect( SFX_DEATH7 );
    mmLoadEffect( SFX_FUTILITY1 );
    mmLoadEffect( SFX_FUTILITY2 );
    mmLoadEffect( SFX_FUTILITY3 );
    mmLoadEffect( SFX_FUTILITY4 );
    mmLoadEffect( SFX_FUTILITY5 );
    mmLoadEffect( SFX_HIT1 );
    mmLoadEffect( SFX_MISSILE );
    mmLoadEffect( SFX_SPINNER );
    mmLoadEffect( SFX_UFO );
    mmLoadEffect( SFX_HYPER ); 
}

void SFX::deinit() {
    mmStop();
    mmEffectCancelAll();
    // Unload all the SFX
    mmUnloadEffect( SFX_DEATH1 );
    mmUnloadEffect( SFX_DEATH2 );
    mmUnloadEffect( SFX_DEATH3 );
    mmUnloadEffect( SFX_DEATH4 );
    mmUnloadEffect( SFX_DEATH5 );
    mmUnloadEffect( SFX_DEATH6 );
    mmUnloadEffect( SFX_DEATH7 );
    mmUnloadEffect( SFX_FUTILITY1 );
    mmUnloadEffect( SFX_FUTILITY2 );
    mmUnloadEffect( SFX_FUTILITY3 );
    mmUnloadEffect( SFX_FUTILITY4 );
    mmUnloadEffect( SFX_FUTILITY5 );
    mmUnloadEffect( SFX_HIT1 );
    mmUnloadEffect( SFX_MISSILE );
    mmUnloadEffect( SFX_SPINNER );
    mmUnloadEffect( SFX_UFO );
    mmUnloadEffect( SFX_HYPER ); 
}

void SFX::update() {
    spinner_min_pitch = 2.0;
}

void SFX::futility() {
    if(muted) return;
    //mmEffectCancel( futility_h );
    //mmEffectRelease( futility_h );
    futility_sf.handle = futility_h;
    futility_sf.id = FUTILITY[ RAND(FUTILITY_FX_COUNT) ];
    futility_h = mmEffectEx( &futility_sf );
}

void SFX::death() {
    if(muted) return;
    mmEffectCancelAll();
    /*
    mmEffectRelease(hit_h);
    mmEffectRelease(futility_h);
    mmEffectRelease(spinner_h);
    mmEffectRelease(ufo_h);
    mmEffectRelease(missile_h);
    */
    death_sf.handle = death_h;
    death_sf.id = DEATH[ RAND(DEATH_FX_COUNT) ];
    death_h = mmEffectEx( &death_sf );
}

void SFX::hit() {
    if(muted) return;
    if(hit_h != -1)
        mmEffectCancel(hit_h);
    //mmEffectRelease(hit_h);
    hit_sf.handle = hit_h;
    hit_sf.id = HIT[ RAND(HIT_FX_COUNT) ];
    hit_h = mmEffectEx( &hit_sf );
}

void SFX::missile_beep() {
    if(muted) return;
    missile_beep_stop();
    missile_sf.handle = missile_h;
    missile_h = mmEffectEx( &missile_sf );
}

// Stops a single missile beep.
void SFX::missile_beep_stop() {
    if(muted) return;
    mmEffectCancel(missile_h);
    //mmEffectRelease(missile_h);
}

void SFX::spinner(float pitch) {
    if(muted) return;
    if(pitch < spinner_min_pitch)
    {
        if(pitch == 1.0)
        {
            spinner_sf.handle = spinner_h;
            spinner_h = mmEffectEx( &spinner_sf );
        }
        spinner_min_pitch = pitch;
        mmEffectRate(spinner_h, static_cast<int>(pitch*pitch*1024));
    }
}

void SFX::spinner_stop() {
    if(muted) return;
    mmEffectCancel(spinner_h);
    //mmEffectRelease(spinner_h);
    spinner_min_pitch = 2.0;
}

void SFX::ufo() {
    if(muted) return;
    mmEffectCancel(ufo_h);
    //mmEffectRelease(ufo_h);
    ufo_sf.handle = ufo_h;
    ufo_sf.volume = 64;
    ufo_h = mmEffectEx( &ufo_sf );
}

void SFX::ufo_stop() {
    if(muted) return;
    mmEffectCancel(ufo_h);
    //mmEffectRelease(ufo_h);
}

void SFX::ufo_beep() {
    if(muted) return;
    ufo_sf.handle = ufo_h;
    ufo_sf.volume = 255;
    ufo_h = mmEffectEx( &ufo_sf );
    //mmEffectVolume(ufo_h,255);
}

void SFX::ufo_beep_stop() {
    if(muted) return;
    ufo_sf.handle = ufo_h;
    ufo_sf.volume = 64;
    ufo_h = mmEffectEx( &ufo_sf );
    //mmEffectVolume(ufo_h,64);
}

void SFX::hyper() {
    if(muted) return;
    hyper_sf.handle = hyper_h;
    hyper_h = mmEffectEx( &hyper_sf );
}

void SFX::mute() {
    mmEffectCancelAll();
    muted = true;
}

void SFX::unmute() {
    muted = false;
}


