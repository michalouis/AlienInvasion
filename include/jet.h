#pragma once

#include "raylib.h"
#include "ADTSet.h"
#include "time.h"
#include "state.h"

typedef struct jet {
    Rectangle rect;
    int hearts;
    bool hit;
    time_t invis_t_start;
}* Jet;

Jet jet_create(float x, float y, float width, float height);

void jet_update(Jet jet, float camera_y, float speed, KeyState keys, Set objects);

// void jet_collision(Set, Jet jet);

// void jet_hit(Jet jet);

bool jet_gameover(Jet jet);

Jet jet_reset(Jet jet, float x, float y);

void jet_destroy(Jet jet);