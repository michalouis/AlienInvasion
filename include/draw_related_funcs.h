#pragma once

// #include "state.h"
#include "raylib.h"
#include <stdlib.h>

typedef struct text_info {
	const char* text;
	Vector2 pos;
	int fontSize;
	Color color;
}* TextInfo;

typedef struct texture_info {
	Vector2 pos;
	Rectangle rect;
	Color color;
}* TextureInfo;

typedef struct animation_info {
	Vector2 pos;
	float frameWidth;
	int maxFrames;
	float timer;
	// float change_frame_t;
	int curr_frame;
}* AnimationInfo;

typedef struct animation {
    Texture texture;
    AnimationInfo info;
}* Animation;

TextInfo create_text(char* content, Vector2 pos, bool center_pos, int size, Color color);

TextureInfo create_texture_info(Vector2 pos, bool center_pos, Rectangle rect, Color color);

Animation create_animation(Texture texture, Vector2 pos, int frames);

void animate(Animation anim, Vector2 pos, float change_frame_t, bool loop);