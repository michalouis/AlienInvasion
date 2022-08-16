#pragma once

#include "raylib.h"
#include <stdlib.h>

typedef struct texture_info {
	Vector2 pos;
	Rectangle rect;
	Color color;
}* TextureInfo;

typedef struct animation {
    Texture texture;
    Vector2 pos;
	float frameWidth;
	int maxFrames;
	float timer;
	int curr_frame;
}* Animation;


///// Texture functions /////

TextureInfo textureInfo_create(Vector2 pos, bool center_pos, Rectangle rect, Color color);

void textureInfo_destroy(TextureInfo texture_info);

///// Animation functions /////

Animation animation_create(Texture texture, int frames);

// Manages animation & updates frames

bool animation_animate(Animation anim, Vector2 pos, float change_frame_t, Color color, bool loop);

// if all frames of the animation have been drawn, return true

bool animation_finished(Animation anim, float change_frame_t);

// Draw animation from the start

void animation_reset(Animation anim);

// Unload animation

void animation_destroy(Animation anim);
