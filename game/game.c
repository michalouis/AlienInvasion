#include <stdio.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"

State state;


void update_and_draw() {

	// Checks which keys are down or pressed
	struct key_state keys = {
		.up = IsKeyDown(KEY_UP),
		.down = IsKeyDown(KEY_DOWN),
		.left = IsKeyDown(KEY_LEFT),
		.right = IsKeyDown(KEY_RIGHT),
		.enter = IsKeyPressed(KEY_ENTER),
		.space = IsKeyDown(KEY_SPACE),
		.p = IsKeyPressed(KEY_P),
		.n = IsKeyDown(KEY_N),
	};

	// Update state for each frame
	// and then draw game
	state_update(state, &keys);
	interface_draw_frame(state, &keys);
}

int main() {
	state = state_create();
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	state_destroy(state);

	interface_close();

	return 0;
}