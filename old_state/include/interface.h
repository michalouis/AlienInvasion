#pragma once

#include "state.h"

typedef struct {
    float x,    //The stars coordinates
          y,    //on the screen
          z;    //The stars depth or distance from camera
}Star;

typedef struct animation {
	Texture texture;
	float frameWidth;
	int maxFrames;
	float timer;
	float change_frame_t;
	int frame;
    bool repeat;
}* Animation;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state, KeyState keys);