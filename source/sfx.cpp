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
bool SFX::muted = false;

void SFX::futility() {
    if(muted) return;
    futility_h = mmEffect( FUTILITY[ RAND(FUTILITY_FX_COUNT) ] );
}

void SFX::death() {
    if(muted) return;
    mmEffectCancelAll();
    death_h = mmEffect( DEATH[ RAND(DEATH_FX_COUNT) ] );
}

void SFX::hit() {
    if(muted) return;
    mmEffectCancel(futility_h);
    mmEffectCancel(hit_h);
    hit_h = mmEffect( HIT[ RAND(HIT_FX_COUNT) ] );
}
