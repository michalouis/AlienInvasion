#include "state.h"
#include "raylib.h"
// #include "interface.h"
#include "draw_related_funcs.h"
#include"title_screen.h"

#include <stdlib.h>
// #include <stdio.h>

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
    
    // Load file with the title screen assets
    title_info->asset_sheet = LoadTexture(
		"assets/title_scr_asset_sheet.png"
	);

    Vector2 position;   // used for position on screen
    Rectangle rect; // used for asset position in asset_sheet
    
    // Create button1
    position = (Vector2){SCREEN_WIDTH / 2, 250};
    rect = (Rectangle){0, 0, 160, 80};
    title_info->button1 = create_texture_info(
        position, true,
        rect,
        WHITE
    );

    // Create button2
    position = (Vector2){SCREEN_WIDTH / 2, 400};
    rect = (Rectangle){0, 80, 160, 80};
    title_info->button2 = create_texture_info(
        position, true,
        rect,
        WHITE
    );

    // ------------------------------------------------------

    title_info->button_selected = 0;    // Which button is selected
    title_info->button_pressed = false; // Is the button pressed?

    // Title Screen text
    position = (Vector2){SCREEN_WIDTH / 2, 100};
    title_info->title_text = create_text(
        "TO PAIXNIDI",
        position, true,
        50,
        WHITE
    );

    return title_info;
}

void destroy_title_scr(State state) {
    TitleScreen title_scr = state->title_screen;

    destroy_texture_info(title_scr->button1);
    destroy_texture_info(title_scr->button2);
    UnloadTexture(title_scr->asset_sheet);
    
    destroy_text(title_scr->title_text);

    free(title_scr);

    state->title_screen = NULL;
}

void title_screen(State state, KeyState keys) {
    if (state->title_screen == NULL)
        state->title_screen = create_title_screen();
    else
        state_update_title_scr(state->title_screen, keys);

    if (state->title_screen->button_pressed)
        if (state->title_screen->button_selected == START) {
            destroy_title_scr(state);
            state->name = START_GAME;
        }
}

void title_screen_draw(TitleScreen title_screen) {
    // Draw text
	DrawText(
		title_screen->title_text->text,
		title_screen->title_text->pos.x,
		title_screen->title_text->pos.y,
		title_screen->title_text->fontSize,
		title_screen->title_text->color
	);

	// Draw Buttons
	DrawTextureRec(
		title_screen->asset_sheet,
		title_screen->button1->rect,
		title_screen->button1->pos,
		title_screen->button_selected ? WHITE : YELLOW
	);

	DrawTextureRec(
		title_screen->asset_sheet,
		title_screen->button2->rect,
		title_screen->button2->pos,
		title_screen->button_selected ? YELLOW : WHITE
	);
}