#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "interface.h"
#include "title_screen.h"
#include "game_screen.h"
#include "draw_related_funcs.h"

State state_create() {
    State state = malloc(sizeof(*state));

    state->name = TITLE_SCREEN;
    state->title_screen = NULL;
    state->game_screen = NULL;

    return state;
}

void* state_info(State state) {
    switch (state->name) {
        case TITLE_SCREEN:
            return state->title_screen;
            break;
        case START_GAME:
            return state->game_screen;
            break;
        case CHARACTER_SELECT:
            break;
    }
    
    return NULL;
}

void state_update(State state, KeyState keys) {
    switch (state->name) {
        case TITLE_SCREEN:
            title_screen(state, keys);
            break;
        case START_GAME:
            game_screen(state, keys);
            break;
        case CHARACTER_SELECT:
            break;
    }
}

// void state_destroy(State state) {
//     switch (state->name) {
//         case TITLE_SCREEN:
//         {
//             TitleScreen title_scr = state_info(state);

//             free(title_scr->title_text);

//             free(title_scr->button1->texture_info);
//             free(title_scr->button1);
//             free(title_scr->button2->texture_info);
//             free(title_scr->button2);

//             free(title_scr);
//         }
//         case START_GAME:
//             break;
//         case CHARACTER_SELECT:
//             break;
//     }

//     free(state);
// }
