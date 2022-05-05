#pragma once

#include "raylib.h"
#include "state.h"

typedef struct {
    float x,    //The stars coordinates
          y,    //on the screen
          z;    //The stars depth or distance from camera
}Star;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state, KeyState keys);

// Animation init_start_game();