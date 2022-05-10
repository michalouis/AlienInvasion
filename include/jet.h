#pragma once

#include "raylib.h"
#include "time.h"

typedef struct jet {
    Rectangle rect;
    int hearts;
    bool hit;
    time_t invis_t_start;
}* Jet;

Jet jet_create(float x, float y, float width, float height);

void jet_hit(Jet jet);

bool jet_gameover(Jet jet);

Jet jet_reset(Jet jet, float x, float y);

void jet_destroy(Jet jet);