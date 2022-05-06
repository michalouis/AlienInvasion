#pragma once

// #include "state.h"
#include "draw_related_funcs.h"

typedef enum {
    START, KATI
}* Buttons;

typedef struct textures{
	Texture asset_sheet;
	TextureInfo button1;
	TextureInfo button2;
}* Textures;

typedef struct title_screen {
	Textures textures;
	int button_selected;
	bool button_pressed;
	TextInfo title_text;
}* TitleScreen;

void title_screen(State state, KeyState keys);

void title_screen_draw(TitleScreen title_screen);