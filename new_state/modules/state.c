#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "state_update_title_scr.h"

TitleScreen create_title_scr() {
    TitleScreen title_info = malloc(sizeof(TitleScreen));

    title_info->button_selected = 0;

    Button b1 = malloc(sizeof(Button));

    b1->posX = 300;
    b1->posY = 250;
    b1->color = YELLOW;
    b1->pressed = false;

    title_info->button1 = b1;

    Button b2 = malloc(sizeof(Button));

    b2->posX = 300;
    b2->posY = 400;
    b2->color = WHITE;
    b2->pressed = false;

    title_info->button2 = b2;

    TextInfo text = malloc(sizeof(TextInfo));

    text->text = "TO PAIXNIDI";
    text->posX = 400;
    text->posY = 100;
    text->fontSize = 50;
    text->color = WHITE;

    title_info->game_title_text = text;

    return title_info;
}

State state_create() {
    State state = malloc(sizeof(State));

    state->name = TITLE_SCREEN;
    state->info.title_scr_info = create_title_scr();

    return state;
}

void* state_info(State state) {
    switch (state->name) {
        case TITLE_SCREEN:
            return state->info.title_scr_info;
        case START_GAME:
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
            break;
        case START_GAME:
            break;
        case CHARACTER_SELECT:
            break;
    }
}