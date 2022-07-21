#include "interface.h"
#include "raylib.h"
#include "state.h"
#include "title_screen.h"
#include "game_screen_draw.h"
#include "ADTList.h"

#include <time.h>
#include <stdlib.h>

#define STARS 250
#define METEORITES 20
#define SCROLL_SPEED_S 2
#define SCROLL_SPEED_M 3

Star stars[STARS];
Meteorite meteorites[METEORITES];

Texture meteorites_asset_sheet;

Music music;

float randf() {
    return (float)GetRandomValue(1, 1000) / 1000.0f;
}

void interface_init() {
	//-------INITIALIZE WINDOW-------//
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Alien Invasion");
	SetTargetFPS(60);

	InitAudioDevice();
	music = LoadMusicStream("assets/music.mp3");

	// Load meteorites for background
	meteorites_asset_sheet = LoadTexture("assets/meteorites.png");

	Rectangle meteorite1 = {0, 0, 55, 53};
	Rectangle meteorite2 = {55, 0, 30, 31};
	Rectangle meteorite3 = {85, 0, 12, 12};
	Rectangle meteorite4 = {0 ,53, 48, 42};
	Rectangle meteorite5 = {48, 53, 18, 26};
	Rectangle meteorite6 = {66, 53, 21, 21};

    //-----RANDOMISE THE STARS POSITIONS-----//
	srand(time(0));
    for (int i = 0; i < STARS; i++) { 
        stars[i].x = GetRandomValue(0, SCREEN_WIDTH);
        stars[i].y = GetRandomValue(0, SCREEN_HEIGHT);
        stars[i].speed = randf();
    }

	for (int i = 0; i < METEORITES; i++) { 
        meteorites[i].x = GetRandomValue(-5, SCREEN_WIDTH);
        meteorites[i].y = GetRandomValue(0, SCREEN_HEIGHT);
        meteorites[i].speed = randf();
		
		int random_meteorite = GetRandomValue(0, 5);
		if (random_meteorite == 0)
			meteorites[i].rect = meteorite1;
		else if (random_meteorite == 1)
			meteorites[i].rect = meteorite2;
		else if (random_meteorite == 2)
			meteorites[i].rect = meteorite3;
		else if (random_meteorite == 3)
			meteorites[i].rect = meteorite4;
		else if (random_meteorite == 4)
			meteorites[i].rect = meteorite5;
		else if (random_meteorite == 5)
			meteorites[i].rect = meteorite6;
    }
}

void interface_close() {
	UnloadTexture(meteorites_asset_sheet);
	UnloadMusicStream(music);

	CloseWindow();
	CloseAudioDevice();
}

// Draw game (one frame)
void interface_draw_frame(State state, KeyState keys) {
    UpdateMusicStream(music);

	///// MOVE THE STARS /////
    for (int i = 0; i < STARS; i++) {
        stars[i].y += SCROLL_SPEED_S * (stars[i].speed / 1);
 
        if (stars[i].y >= SCREEN_HEIGHT) {  // Check if the star has gone off screen
            stars[i].y -= SCREEN_HEIGHT;
            stars[i].x = GetRandomValue(0, SCREEN_WIDTH);
        }
    }
        
	///// MOVE THE METEORITES /////
	for (int i = 0; i < METEORITES; i++) {
        meteorites[i].y += SCROLL_SPEED_M * (meteorites[i].speed / 1);
 
        if (meteorites[i].y >= SCREEN_HEIGHT) {  // Check if the meteorite has gone off screen
            meteorites[i].y -= SCREEN_HEIGHT + meteorites[i].rect.height;
            meteorites[i].x = GetRandomValue(-5, SCREEN_WIDTH);
        }
    }

	BeginDrawing();

	// BACKGROUND
	ClearBackground(BLACK);
        
    ///// DRAW THE STARS /////
    for (int i = 0; i < STARS; i++) {
        float x = stars[i].x;
        float y = stars[i].y;
                
        DrawPixel(x, y, WHITE);
    }

	///// DRAW THE METEORITES /////
	for (int i = 0; i < METEORITES; i++) {
        float x = meteorites[i].x;
        float y = meteorites[i].y;
		Rectangle rect = meteorites[i].rect;
                
        DrawTextureRec(
			meteorites_asset_sheet,
			rect,
			(Vector2) {x, y}, 
			DARKGRAY
		);
    }

	// Draw Screen
	if (state->game_screen != NULL || state->title_screen != NULL) {
		switch (state->name) {
			case TITLE_SCREEN:
			{
				title_screen_draw(state->title_screen);
				break;
			}
			case GAME_SCREEN:
			{
				game_screen_draw(state->game_screen, keys);
				break;
			}

		}
	}

	EndDrawing();
}