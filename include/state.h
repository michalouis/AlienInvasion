#pragma once

#include "raylib.h"
#include "ADTSet.h"
#include "draw_related_funcs.h"

#include <time.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_W_G 880	// width of game screen
#define SCREEN_W_T 400	// width of tab screen

// screen types
typedef enum {
	TITLE_SCREEN, GAME_SCREEN
} StateName;

typedef struct title_screen* TitleScreen;
typedef struct game_screen* GameScreen;

typedef struct state {
	StateName name;
	TitleScreen title_screen;
	GameScreen game_screen;
}* State;

// Struct for pressed keys
typedef struct key_state {
	bool up;
	bool down;
	bool left;
	bool right;
	bool space;
	bool x;
	bool enter;
	bool p;
	bool b;
}* KeyState;

// STATE_CREATE

State state_create();

// STATE_UPDATE

void state_update(State state, KeyState keys);

// STATE_DESTROY

void state_destroy(State state);