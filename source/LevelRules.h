#ifndef _LEVEL_RULES_H
#define _LEVEL_RULES_H

typedef struct LevelRules {
    // Maximum number of enemies active on the screen, for each type.
    int max_rocks;
    int max_spinners;
    int max_missiles;
    int max_ufos;

    // Min and Max length of time between generation of each enemy type.
    int max_rock_spawn; // Max number of rocks to spawn at a time
    int min_rock_rest;
    int max_rock_rest;

    int max_spinner_spawn;
    int min_spinner_rest;
    int max_spinner_rest;

    int min_missile_rest;
    int max_missile_rest;

    int min_ufo_rest;
    int max_ufo_rest;

    float ufo_speed;

    bool allow_missile_while_ufo;
} LevelRules;

static LevelRules X1_RULES = {
    8,      // max_rocks
    2,      // max_spinners
    1,      // max_missiles
    0,      // max_ufos
    
    3,      // max_rock_spawn
    0,      // min_rock_rest
    3000,   // max_rock_rest
    
    1,      // max_spinner_spawn
    2000,   // min_spinner_rest
    15000,  // max_spinner_rest

    15000,  // min_missile_rest
    60000,  // max_missile_rest

    2000,   // min_ufo_rest
    15000,  // max_ufo_rest
    
    0.5,    // ufo_speed

    false   // allow_missile_while_ufo
};

static LevelRules X2_RULES = {
    8,      // max_rocks
    2,      // max_spinners
    1,      // max_missiles
    0,      // max_ufos
    
    3,      // max_rock_spawn
    0,      // min_rock_rest
    3000,   // max_rock_rest
    
    2,      // max_spinner_spawn
    1000,   // min_spinner_rest
    12000,  // max_spinner_rest

    10000,  // min_missile_rest
    45000,  // max_missile_rest

    2000,   // min_ufo_rest
    15000,  // max_ufo_rest
    
    0.5,    // ufo_speed

    false   // allow_missile_while_ufo
};

static LevelRules X3_RULES = {
    12,     // max_rocks
    3,      // max_spinners
    1,      // max_missiles
    0,      // max_ufos
    
    3,      // max_rock_spawn
    0,      // min_rock_rest
    2200,   // max_rock_rest
    
    3,      // max_spinner_spawn
    500,    // min_spinner_rest
    8500,   // max_spinner_rest

    8000,   // min_missile_rest
    30000,  // max_missile_rest

    2000,   // min_ufo_rest
    15000,  // max_ufo_rest
    
    0.5,    // ufo_speed

    false   // allow_missile_while_ufo
};

static LevelRules X4_RULES = {
    14,      // max_rocks
    4,      // max_spinners
    1,      // max_missiles
    1,      // max_ufos
    
    3,      // max_rock_spawn
    0,      // min_rock_rest
    1800,   // max_rock_rest
    
    3,      // max_spinner_spawn
    250,    // min_spinner_rest
    6500,   // max_spinner_rest

    8000,   // min_missile_rest
    30000,  // max_missile_rest

    2000,   // min_ufo_rest
    15000,  // max_ufo_rest
    
    0.5,    // ufo_speed

    false   // allow_missile_while_ufo
};

static LevelRules X5_RULES = {
    18,     // max_rocks
    5,      // max_spinners
    1,      // max_missiles
    1,      // max_ufos
    
    3,      // max_rock_spawn
    0,      // min_rock_rest
    1000,   // max_rock_rest
    
    3,      // max_spinner_spawn
    100,    // min_spinner_rest
    5000,   // max_spinner_rest

    8000,   // min_missile_rest
    30000,  // max_missile_rest

    2000,   // min_ufo_rest
    15000,  // max_ufo_rest
    
    0.5,    // ufo_speed

    false   // allow_missile_while_ufo
};

static LevelRules X6_RULES = {
    18,     // max_rocks
    5,      // max_spinners
    1,      // max_missiles
    1,      // max_ufos
    
    3,      // max_rock_spawn
    0,      // min_rock_rest
    800,    // max_rock_rest
    
    4,      // max_spinner_spawn
    0,      // min_spinner_rest
    4000,   // max_spinner_rest

    8000,   // min_missile_rest
    20000,  // max_missile_rest

    2000,   // min_ufo_rest
    15000,  // max_ufo_rest
    
    0.5,    // ufo_speed

    false   // allow_missile_while_ufo
};

#endif
