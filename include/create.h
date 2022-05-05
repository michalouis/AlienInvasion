#pragma once

#include "state.h"


Button create_button(TextureInfo texture_info, bool pressed);

TextInfo create_text(char* content, Vector2 pos, int size, Color color);

TextureInfo create_texture_info(Vector2 pos, Rectangle rect, Color color);

Animation create_animation(Texture texture, Vector2 pos, int frames, float change_frame_t);