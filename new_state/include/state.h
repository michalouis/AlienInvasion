#pragma once

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef enum {
	TITLE_SCREEN, START_GAME, CHARACTER_SELECT
} StateName;

typedef struct text_info {
	const char* text;
	int posX;
	int posY;
	int fontSize;
	Color color;
}* TextInfo;

typedef struct texture_info {
	int posX;
	int posY;
	Color color;
}* TextureInfo;

typedef struct button {
	// struct texture_info button_texture;
	int posX;
	int posY;
	Color color;
	bool pressed;
}* Button;

typedef struct title_scr {
	TextInfo game_title_text;
	int button_selected;
	Button button1;
	Button button2;
}* TitleScreen;

// typedef struct start_game {

// }* StartGame;

// typedef struct char_select {

// }* CharSelect;

typedef struct state_info {
	TitleScreen title_scr_info;
	// struct start_game start_game;
	// struct char_select char_select;
}* StateInfo;

typedef struct state {
	StateName name;
	struct state_info info;
}* State;

// Πληροφορίες για το ποια πλήκτρα είναι πατημένα
typedef struct key_state {
	bool up;
	bool down;
	bool left;
	bool right;
	bool enter;
}* KeyState;

// STATE_CREATE

State state_create();

// STATE_INFO

void* state_info(State state);

// STATE_UPDATE

void state_update(State state, KeyState keys);