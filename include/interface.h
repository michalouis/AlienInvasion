#pragma once

#include "raylib.h"
#include "state.h"

typedef struct {
    float x;    // The stars coordinates
    float y;    // on the screen
    float speed;    // Star speed
}Star;

typedef struct {
    Rectangle rect;
    float x;    // The meteorite coordinates
    float y;    // on the screen
    float speed;    // Meteorite speed
}Meteorite;

// initialize interface
void interface_init();

// close interface
void interface_close();

// draws frame
void interface_draw_frame(State state, KeyState keys);