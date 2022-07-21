#include <stdlib.h>

#include "state.h"
#include "title_screen.h"
#include "game_screen.h"

State state_create() {
    State state = malloc(sizeof(*state));

    state->name = TITLE_SCREEN;
    state->title_screen = NULL;
    state->game_screen = NULL;

    return state;
}

void state_update(State state, KeyState keys) {
    switch (state->name) {
        case TITLE_SCREEN:
            title_screen(state, keys);
            break;
        case GAME_SCREEN:
            game_screen(state, keys);
            break;
    }
}

void state_destroy(State state) {
    if (state->title_screen != NULL)
        title_screen_destroy(state);
    
    if (state->game_screen != NULL)
        destroy_game_screen(state);

    free(state);
}
