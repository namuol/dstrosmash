#include "sfx.h"
#include "util.h"
#include "soundbank.h"

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

mm_sfxhand SFX::futility_h = 0;
mm_sfxhand SFX::death_h = 0;
mm_sfxhand SFX::hit_h = 0;
mm_sfxhand SFX::missile_h = 0; 
mm_sfxhand SFX::spinner_h = 0; 

float SFX::spinner_min_pitch = 2.0;
bool SFX::muted = false;
int SFX::missiles_playing = 0;
int SFX::spinners_playing = 0;

void SFX::update() {
    spinner_min_pitch = 2.0;
}

void SFX::futility() {
    if(muted) return;
    futility_h = mmEffect( FUTILITY[ RAND(FUTILITY_FX_COUNT) ] );
}

void SFX::death() {
    if(muted) return;
    missiles_playing = 0;
    mmEffectCancelAll();
    mmSetModuleVolume( 0 );
    death_h = mmEffect( DEATH[ RAND(DEATH_FX_COUNT) ] );
}

void SFX::hit() {
    if(muted) return;
    mmEffectCancel(futility_h);
    mmEffectCancel(hit_h);
    hit_h = mmEffect( HIT[ RAND(HIT_FX_COUNT) ] );
}

void SFX::missile_beep() {
    if(muted) return;
    missile_beep_cancel();
    missile_h = mmEffect( SFX_MISSILE );
}

// Stops a single missile beep.
void SFX::missile_beep_cancel() {
    mmEffectCancel(missile_h);
}

void SFX::missile_start() {
/*
    if(muted) return;
    ++missiles_playing;
    mmSetModuleVolume( 1024 );
*/
}

void SFX::missile_stop() {
/*
    --missiles_playing;
    if(missiles_playing <= 0)
        mmSetModuleVolume( 0 );
*/
}

void SFX::spinner(float pitch) {
    if(muted) return;
    if(pitch < spinner_min_pitch)
    {
        spinner_min_pitch = pitch;
        if(pitch == 1.0)
        {
            mmEffectCancel(spinner_h);
            spinner_h = mmEffect(SFX_SPINNER);
        }
        mmEffectRate(spinner_h, static_cast<int>(pitch*pitch*1024));
    }
}

void SFX::spinner_cancel() {
    mmEffectCancel(spinner_h);
    spinner_min_pitch = 2.0;
}

void SFX::spinner_start() {
    if(muted) return;
    ++spinners_playing;
    mmSetJingleVolume( 1024 );
    mmJingle( MOD_SPINNER );
}

void SFX::spinner_stop() {
    --spinners_playing;
    if(spinners_playing <= 0)
        mmSetJingleVolume( 0 );
}
