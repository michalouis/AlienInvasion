#include "raylib.h"
#include "state.h"
#include "interface.h"

#include <stdio.h>

State state;

void update_and_draw() {

    // Checks which keys are down or pressed
	struct key_state keys = {
		.up     = IsKeyPressed(KEY_UP),
		.down   = IsKeyPressed(KEY_DOWN),
		.left   = IsKeyPressed(KEY_LEFT),
		.right  = IsKeyPressed(KEY_RIGHT),
		.enter  = IsKeyPressed(KEY_ENTER),
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