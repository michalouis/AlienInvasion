#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "interface.h"
#include "state_update_title_scr.h"
#include "start_game.h"
#include "create.h"

TitleScreen create_title_screen() {
    // Allocate memory for TitleScreen room
    TitleScreen title_info = malloc(sizeof(*title_info));

    Vector2 position;   // used for position on screen
    
    // Title Screen text
    position = (Vector2){SCREEN_WIDTH / 2, 100};
    title_info->title_text = create_text(
        "TO PAIXNIDI",
        position,
        50,
        WHITE
    );

    // Selected button
    title_info->button_selected = 0;
    
    // Load file with the title screen assets
    title_info->asset_sheet = LoadTextureFromImage(
		LoadImage("assets/title_scr_asset_sheet.png")
	);


    Rectangle rect; // used for asset position in asset_sheet
    
    // Create button1
    position = (Vector2){SCREEN_WIDTH / 2 -80, 250};
    rect = (Rectangle){0, 0, 160, 80};

    title_info->button1 = create_button(
        create_texture_info(
            position,
            rect,
            YELLOW
        ),
        false
    );
    
    // Create button2
    position = (Vector2){SCREEN_WIDTH / 2 -80, 400};
    rect = (Rectangle){0, 80, 160, 80};

    title_info->button2 = create_button(
        create_texture_info(
            position,
            rect,
            WHITE
        ),
        false
    );

    return title_info;
}

State state_create() {
    State state = malloc(sizeof(*state));

    state->name = TITLE_SCREEN;
    state->info.title_screen = create_title_screen();
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
            state_update_title_scr(state_info(state), keys);
            if (state->info.title_screen->button1->pressed) {
                destroy_title_scr(state);
                state->name = START_GAME;
                state->info.start_game = create_start_game();
            }
            break;
        case START_GAME:
            start_game_update(state_info(state), keys);
            break;
        case CHARACTER_SELECT:
            break;
    }
}

void state_destroy(State state) {
    switch (state->name) {
        case TITLE_SCREEN:
        {
            TitleScreen title_scr = state_info(state);

            free(title_scr->title_text);

            free(title_scr->button1->texture_info);
            free(title_scr->button1);
            free(title_scr->button2->texture_info);
            free(title_scr->button2);

            free(title_scr);
        }
        case START_GAME:
            break;
        case CHARACTER_SELECT:
            break;
    }

    free(state);
}

void destroy_title_scr(State state) {
    TitleScreen title_scr = state_info(state);

    free(title_scr->title_text);

    free(title_scr->button1->texture_info);
    free(title_scr->button1);
    free(title_scr->button2->texture_info);
    free(title_scr->button2);

    free(title_scr);
}