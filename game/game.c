#include "raylib.h"
#include "interface.h"
#include "state.h"

#include <stdio.h>

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

int main(void) {
    interface_init();
    state = state_create();

    start_main_loop(update_and_draw);

    interface_close();
	state_destroy(state);

    return 0;
}