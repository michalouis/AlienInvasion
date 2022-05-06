#include "state.h"
#include "raylib.h"
#include "interface.h"
#include "create.h"
#include"title_screen.h"

#include <stdlib.h>
#include <stdio.h>

void state_update_title_scr(TitleScreen title_scr, KeyState keys) {
    if (keys->up) title_scr->button_selected--;
    
    if (title_scr->button_selected < 0)
        title_scr->button_selected = 0;

    if (keys->down) title_scr->button_selected++;

    if (title_scr->button_selected > 1)
        title_scr->button_selected = 1; 

    if (keys->enter)
        title_scr->button_pressed = true;
    else
        title_scr->button_pressed = false;
}

TitleScreen create_title_screen() {
    // Allocate memory for TitleScreen room
    TitleScreen title_info = malloc(sizeof(*title_info));
    
    // CREATE TITLE SCREEN TEXTURES
    // ------------------------------------------------------
    // Allocate memory for title screen textures
    Textures textures = malloc(sizeof(*textures));
    
    // Load file with the title screen assets
    textures->asset_sheet = LoadTextureFromImage(
		LoadImage("assets/title_scr_asset_sheet.png")
	);

    Vector2 position;   // used for position on screen
    Rectangle rect; // used for asset position in asset_sheet
    
    // Create button1
    position = (Vector2){SCREEN_WIDTH / 2 - 80, 250};
    rect = (Rectangle){0, 0, 160, 80};
    textures->button1 = create_texture_info(
        position,
        rect,
        WHITE
    );

    // Create button2
    position = (Vector2){SCREEN_WIDTH / 2 - 80, 400};
    rect = (Rectangle){0, 80, 160, 80};
    textures->button2 = create_texture_info(
        position,
        rect,
        WHITE
    );

    title_info->textures = textures;
    // ------------------------------------------------------

    title_info->button_selected = 0;    // Which button is selected
    title_info->button_pressed = false; // Is the button pressed?

    // Title Screen text
    position = (Vector2){SCREEN_WIDTH / 2, 100};
    title_info->title_text = create_text(
        "TO PAIXNIDI",
        position,
        50,
        WHITE
    );

    return title_info;
}

void destroy_title_scr(State state) {
    TitleScreen title_scr = state->info.title_screen;

    free(title_scr->textures->button1);
    free(title_scr->textures->button2);
    free(title_scr->textures);
    
    free(title_scr->title_text);

    free(title_scr);

    state->info.title_screen = NULL;
}

void title_screen(State state, KeyState keys) {
    if (state->info.title_screen == NULL)
        state->info.title_screen = create_title_screen();
    else
        state_update_title_scr(state->info.title_screen, keys);

    if (state->info.title_screen->button_pressed)
        if (state->info.title_screen->button_selected == START) {
            destroy_title_scr(state);
            state->name = START_GAME;
        }
}