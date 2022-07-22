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
		.space = IsKeyPressed(KEY_SPACE),
		.z = IsKeyDown(KEY_Z),
		.x = IsKeyPressed(KEY_X),
		.enter = IsKeyPressed(KEY_ENTER),
		.p = IsKeyPressed(KEY_P),
		.b = IsKeyPressed(KEY_B)
	};


    // Update state for each frame
	// and then draw game
	state_update(state, &keys);
	interface_draw_frame(state, &keys);
}

int main(void) {
    state = state_create();
    interface_init();

    start_main_loop(update_and_draw);

    interface_close();
	state_destroy(state);

    return 0;
}