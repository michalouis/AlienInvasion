#include "raylib.h"
#include "state.h"
#include "draw_related_funcs.h"
#include "title_screen.h"
#include "start_game.h"
#include "interface.h"
#include "ADTList.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define STARS 200
#define METEORITES 15
#define SCROLL_SPEED_S 2
#define SCROLL_SPEED_M 3

Star stars[STARS];
Meteorite meteorites[METEORITES];

Texture meteorites_asset_sheet;

float randf() {
    return (rand() % 1000) / 1000.0f;
}

int randf_meteorites() {
    return rand() % 5;
}

void interface_init() {
	//-------INITIALIZE WINDOW-------//
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

	meteorites_asset_sheet = LoadTextureFromImage(
		LoadImage("assets/meteorites.png")
	);

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
        stars[i].z = randf();
    }

	for (int i = 0; i < METEORITES; i++) { 
        meteorites[i].x = GetRandomValue(-5, SCREEN_WIDTH);
        meteorites[i].y = GetRandomValue(0, SCREEN_HEIGHT);
        meteorites[i].z = randf();
		
		if (randf_meteorites() == 0)
			meteorites[i].rect = meteorite1;
		else if (randf_meteorites() == 1)
			meteorites[i].rect = meteorite2;
		else if (randf_meteorites() == 2)
			meteorites[i].rect = meteorite3;
		else if (randf_meteorites() == 3)
			meteorites[i].rect = meteorite4;
		else if (randf_meteorites() == 4)
			meteorites[i].rect = meteorite5;
		else if (randf_meteorites() == 5)
			meteorites[i].rect = meteorite6;
    }
}

void interface_close() {
	// UnloadTexture(button1);
	// UnloadTexture(button2);

	CloseWindow();
}

// Draw game (one frame)
void interface_draw_frame(State state, KeyState keys) {
	BeginDrawing();

	// BACKGROUND
	// -------------------------------------------------------------------------------------
	ClearBackground((Color){0, 0, 0, 255});
	//-----SCROLL THE STARS-----//
        for (int i = 0; i < STARS; i++) {
            stars[i].y += SCROLL_SPEED_S * (stars[i].z / 1);
 
            if (stars[i].y >= SCREEN_HEIGHT) {  // Check if the star has gone off screen
                stars[i].y -= SCREEN_HEIGHT;
                stars[i].x = GetRandomValue(0, SCREEN_WIDTH);
            }
        }
        
		for (int i = 0; i < METEORITES; i++) {
            meteorites[i].y += SCROLL_SPEED_M * (meteorites[i].z / 1);
 
            if (meteorites[i].y >= SCREEN_HEIGHT) {  // Check if the star has gone off screen
                meteorites[i].y -= SCREEN_HEIGHT + meteorites[i].rect.height;
                meteorites[i].x = GetRandomValue(-5, SCREEN_WIDTH);
            }
        }
        
        //-----DRAW THE STARS-----//
        for (int i = 0; i < STARS; i++) {
            float x = stars[i].x;
            float y = stars[i].y;
                
            DrawPixel(x, y, WHITE);
        }

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

	// -------------------------------------------------------------------------------------
	if (state->info.start_game != NULL || state->info.title_screen != NULL) {
	switch (state->name) {
		case TITLE_SCREEN:
		{
			title_screen_draw(state->info.title_screen);
			break;
		}
		case START_GAME:
		{
			start_game_draw(state->info.start_game, keys);
			break;
		}
		case CHARACTER_SELECT:
			break;
	}
	}
	
	// Draw score and FPS counter
	DrawFPS(SCREEN_WIDTH - 80, 0);

	EndDrawing();
}