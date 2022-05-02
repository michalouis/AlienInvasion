#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "state_update_title_scr.h"

Button create_button(int x, int y, Color color, bool pressed) {
    Button b = malloc(sizeof(*b));

    b->posX = x;
    b->posY = y;
    b->pressed = pressed;
    b->color = color;

    return b;
}

TextInfo create_text(char* content, int x, int y, int size, Color color) {
    TextInfo text = malloc(sizeof(*text));

    text->text = content;
    text->posX = x;
    text->posY = y;
    text->fontSize = size;
    text->color = color;

    return text;
}

TitleScreen create_title_scr() {
    TitleScreen title_info = malloc(sizeof(*title_info));

    title_info->button_selected = 0;

    title_info->button1 = create_button(300, 250, YELLOW, false);
    title_info->button2 = create_button(300, 400, WHITE, false);

    title_info->game_title_text = create_text("TO PAIXNIDI", 400, 100, 50, WHITE);

    return title_info;
}

State state_create() {
    State state = malloc(sizeof(*state));

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

void state_destroy(State state) {
    TitleScreen title_scr = state_info(state);

    free(title_scr->game_title_text);

    free(title_scr->button1);
    free(title_scr->button2);

    free(title_scr);

    free(state);
}