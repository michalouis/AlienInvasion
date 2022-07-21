#pragma once

#include "draw_related_funcs.h"

typedef struct title_screen {
	int button_selected;
	bool button_pressed;
	Texture game_title;
	Texture title_screen_buttons;
	Texture how_to_play;
	Texture credits;
	TextureInfo start_game_button;
	TextureInfo how_to_play_button;
	TextureInfo credits_button;
	Sound sound_menu_navigation;
	float timer;
	bool draw_text;
}* TitleScreen;

void title_screen(State state, KeyState keys);

void title_screen_draw(TitleScreen title_screen);

void title_screen_destroy(State state);