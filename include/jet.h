#pragma once

#include "raylib.h"
#include "ADTSet.h"
#include "state.h"

typedef struct jet {
    Rectangle rect;
    int hearts;
    int missiles;
    bool hit;
    bool hit_by_beam;
    float invisibility_time;
    float effect_time;
    bool shield;
    float shield_time;
    float shield_cooldown;
}* Jet;

// Create jet

Jet jet_create(float x, float y, float width, float height);

// Update anything jet related

void jet_update(Jet jet, float camera_y, float speed, KeyState keys, Set objects, Set beams, Sound hit_player, Sound hit_beam);

// if hearts reach zero game is over(return true)

bool jet_gameover(Jet jet);

// Reset jet before restarting game

Jet jet_reset(Jet jet, float x, float y);

// Free memory used by jet

void jet_destroy(Jet jet);