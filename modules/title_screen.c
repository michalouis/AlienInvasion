#include "state.h"
#include "raylib.h"
#include "draw_related_funcs.h"
#include "title_screen.h"

#include <stdlib.h>
#include <stdio.h>

extern Music music;

static void title_screen_update(TitleScreen title_screen, KeyState keys) {
    if (IsKeyPressed(KEY_LEFT)) {
        title_screen->button_selected--;
        PlaySound(title_screen->sound_menu_navigation);
    }
    
    if (title_screen->button_selected < 0)
        title_screen->button_selected = 0;

    if (IsKeyPressed(KEY_RIGHT)) {
        title_screen->button_selected++;
        PlaySound(title_screen->sound_menu_navigation);
    }

    if (title_screen->button_selected > 2)
        title_screen->button_selected = 2; 

    if (keys->enter)
        title_screen->button_pressed = true;
    else
        title_screen->button_pressed = false;
}

TitleScreen create_title_screen() {
    // Allocate memory for TitleScreen room
    TitleScreen title_screen = malloc(sizeof(*title_screen));

    // Initialize title screen values
    title_screen->button_selected = 0;    // Which button is selected
    title_screen->button_pressed = false; // Is the button pressed?
    title_screen->timer = 0;
    title_screen->draw_text = true;
    
    // Load title screen textures
    title_screen->title_screen_buttons = LoadTexture("assets/title_screen/title_screen_buttons.png");
    title_screen->game_title = LoadTexture("assets/title_screen/logo.png");
    title_screen->how_to_play = LoadTexture("assets/title_screen/how_to_play.png");
    title_screen->credits = LoadTexture("assets/title_screen/credits.png");

    Vector2 position;   // used for position on screen
    Rectangle rect; // used for asset position in title_screen_buttons
    
    // Create start_game_button info
    position = (Vector2){SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.8};
    rect = (Rectangle){0, 0, 160, 80};
    title_screen->start_game_button = textureInfo_create(
        position, true,
        rect,
        WHITE
    );

    // Create how_to_play_button info
    position = (Vector2){SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.8};
    rect = (Rectangle){0, 80, 160, 80};
    title_screen->how_to_play_button = textureInfo_create(
        position, true,
        rect,
        WHITE
    );
    
    // Create credits_button
    position = (Vector2){SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.8};
    rect = (Rectangle){0, 160, 160, 80};
    title_screen->credits_button = textureInfo_create(
        position, true,
        rect,
        WHITE
    );

    // Load title screen sounds
    title_screen->sound_menu_navigation = LoadSound("assets/title_screen/menu_navigation.mp3");
    SetSoundVolume(title_screen->sound_menu_navigation, 0.15);

    return title_screen;
}

void title_screen_destroy(State state) {
    TitleScreen title_screen = state->title_screen;

    // Unload textures
    UnloadTexture(title_screen->game_title);
    UnloadTexture(title_screen->title_screen_buttons);
    UnloadTexture(title_screen->how_to_play);
    UnloadTexture(title_screen->credits);

    // free texture info
    textureInfo_destroy(title_screen->start_game_button);
    textureInfo_destroy(title_screen->how_to_play_button);

    // Unload sounds
    UnloadSound(title_screen->sound_menu_navigation);

    free(title_screen);

    state->title_screen = NULL;

    StopMusicStream(music);
}

void title_screen(State state, KeyState keys) {
    if (state->title_screen == NULL)
        state->title_screen = create_title_screen();
    else
        title_screen_update(state->title_screen, keys);

    if (state->title_screen->button_pressed)
        if (state->title_screen->button_selected == 0) {
            title_screen_destroy(state);
            state->name = GAME_SCREEN;
        }
}

void title_screen_draw(TitleScreen title_screen) {
    if (!IsMusicPlaying(music)) {
        PlayMusicStream(music);
    }

    // Draw Title
    DrawTexture(
        title_screen->game_title,
        SCREEN_WIDTH / 2 - title_screen->game_title.width / 2,
        SCREEN_HEIGHT * 0.15,
        RAYWHITE
    );

	// Draw Buttons
	DrawTextureRec(
		title_screen->title_screen_buttons,
		title_screen->start_game_button->rect,
		title_screen->start_game_button->pos,
		title_screen->button_selected == 0 ? (Color) {91, 110, 225, 255} : WHITE
	);

	DrawTextureRec(
		title_screen->title_screen_buttons,
		title_screen->how_to_play_button->rect,
		title_screen->how_to_play_button->pos,
		title_screen->button_selected == 1 ? (Color) {91, 110, 225, 255} : WHITE
	);
	
    DrawTextureRec(
		title_screen->title_screen_buttons,
		title_screen->credits_button->rect,
		title_screen->credits_button->pos,
		title_screen->button_selected == 2 ? (Color) {91, 110, 225, 255} : WHITE
	);

    // Manage flashing text
    title_screen->timer += GetFrameTime();
    if (title_screen->timer > 0.8) {
        title_screen->timer= 0;
        title_screen->draw_text = !title_screen->draw_text;
    }

    if (title_screen->button_selected == 0 && title_screen->draw_text)
        // Draw flashing text
        DrawText(
            "PRESS [ENTER] TO START",
            SCREEN_WIDTH / 2 - MeasureText("PRESS [ENTER] TO START", 30) / 2,
            SCREEN_HEIGHT * 0.65,
            30, GRAY
        );
    else if (title_screen->button_selected == 1)
        // Draw how to play window
        DrawTexture(
            title_screen->how_to_play,
            SCREEN_WIDTH * 0.05,
            SCREEN_HEIGHT * 0.05,
            RAYWHITE
        );
    else if (title_screen->button_selected == 2)
        // Draw credits window
        DrawTexture(
            title_screen->credits,
            SCREEN_WIDTH * 0.05,
            SCREEN_HEIGHT * 0.05,
            RAYWHITE
        );
}