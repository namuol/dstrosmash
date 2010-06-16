#ifndef _LEVEL_RULES_H
#define _LEVEL_RULES_H

struct LevelRules {
    // Maximum number of enemies active on the screen, for each type.
    unsigned int max_rocks;
    unsigned int max_spinners;
    unsigned int max_missiles;
    unsigned int max_ufos;

    // Min and Max length of time between generation of each enemy type.
    unsigned int max_rock_spawn; // Max number of rocks to spawn at a time
    unsigned int min_rock_rest;
    unsigned int max_rock_rest;

    unsigned int max_spinner_spawn;
    unsigned int min_spinner_rest;
    unsigned int max_spinner_rest;

    unsigned int min_missile_rest;
    unsigned int max_missile_rest;

    unsigned int min_ufo_rest;
    unsigned int max_ufo_rest;

    float ufo_speed;

    bool allow_missile_while_ufo;

    static LevelRules RULES[];
};

#endif
