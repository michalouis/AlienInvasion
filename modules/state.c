#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "interface.h"
#include "title_screen.h"
#include "start_game.h"
#include "create.h"

State state_create() {
    State state = malloc(sizeof(*state));

    state->name = TITLE_SCREEN;
    state->info.title_screen = NULL;
    state->info.start_game = NULL;

    return state;
}

void* state_info(State state) {
    switch (state->name) {
        case TITLE_SCREEN:
            return state->info.title_screen;
            break;
        case START_GAME:
            return state->info.start_game;
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
            if (state->info.start_game == NULL)
                state->info.start_game = create_start_game();

            start_game_update(state_info(state), keys);
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
