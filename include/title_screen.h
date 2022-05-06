#pragma once

// #include "state.h"
#include "create.h"

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

void state_update_title_scr(TitleScreen info, KeyState keys);

int WhichButtonIsPressed(TitleScreen info);

TitleScreen create_title_screen();

void destroy_title_scr(State state);

void title_screen(State state, KeyState keys);