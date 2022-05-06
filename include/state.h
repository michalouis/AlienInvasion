#pragma once

#include "raylib.h"
#include "ADTSet.h"
#include "draw_related_funcs.h"

#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576
#define SCREEN_W_G 720
#define SCREEN_W_T 304

typedef enum {
	TITLE_SCREEN, START_GAME, CHARACTER_SELECT
} StateName;

typedef struct title_screen* TitleScreen;
typedef struct start_game* StartGame;

// typedef struct char_select {

// }* CharSelect;

typedef struct state {
	StateName name;
	TitleScreen title_screen;
	StartGame start_game;
	// CharSelect char_select;
	int selected_character;
}* State;

// Πληροφορίες για το ποια πλήκτρα είναι πατημένα
typedef struct key_state {
	bool up;
	bool down;
	bool left;
	bool right;
	bool space;
	bool enter;
	bool n;
	bool p;
}* KeyState;

// STATE_CREATE

State state_create();

// STATE_INFO

void* state_info(State state);

// STATE_UPDATE

void state_update(State state, KeyState keys);

void state_destroy(State state);

void destroy_title_scr(State state);
