#pragma once

#include "state.h"
#include "ADTList.h"

#define BRIDGE_NUM 100		// πόσες γέφυρες δημιουργούνται στην πίστα
#define SPACING 200			// απόσταση ανάμεσα στα αντικείμενα της πίστας

List state_objects(GameState gamestate, float y_from, float y_to);

void start_game(State state, KeyState keys);

void start_game_draw(StartGame start_game, KeyState keys);