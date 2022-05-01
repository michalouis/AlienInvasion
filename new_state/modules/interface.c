#include "raylib.h"
#include "state.h"
#include "interface.h"

#include <time.h>
#include <stdlib.h>

#define STARS 200
#define SCROLL_SPEED 2

Texture button1;
Texture button2;

Star stars[STARS];

float randf() {
    return (rand() % 1000) / 1000.0f;
}

// LOAD TEXTURES
// ---------------------------------------------------------------------
void interface_init() {
	// Initialize window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

	// Load assets
	button1 = LoadTextureFromImage(
		LoadImage("assets/button1.png")
	);
	
    button2 = LoadTextureFromImage(
		LoadImage("assets/button2.png")
	);


	srand(time(0));
    //-----RANDOMISE THE STARS POSITIONS-----//
    for (int i = 0; i < STARS; i++) { 
        stars[i].x = GetRandomValue(0, SCREEN_WIDTH);
        stars[i].y = GetRandomValue(0, SCREEN_HEIGHT);
        stars[i].z = randf();
    }
}
// ---------------------------------------------------------------------

void interface_close() {
	CloseWindow();
}

// Draw game (one frame)
void interface_draw_frame(State state, KeyState keys) {
	BeginDrawing();

	// float scale = 1.0f, rotation = 0.0f;
	
	// BACKGROUND
	// -------------------------------------------------------------------------------------
	ClearBackground((Color){0, 0, 0, 255});
	//-----SCROLL THE STARS-----//
        for (int i = 0; i < STARS; i++) {
            stars[i].y += SCROLL_SPEED * (stars[i].z / 1);
 
            if (stars[i].y >= SCREEN_HEIGHT) {  // Check if the star has gone off screen
                stars[i].y -= SCREEN_HEIGHT;
                stars[i].x = GetRandomValue(0, SCREEN_WIDTH);
            }
        }
        
        //-----DRAW THE STARS-----//
        for (int i = 0; i < STARS; i++) {
            float x = stars[i].x;
            float y = stars[i].y;
                
            DrawPixel(x, y, WHITE);
        }

	// -------------------------------------------------------------------------------------

	TitleScreen info = state_info(state);

    // Draw Game Title
	int x_pixels =
		MeasureText(
			info->game_title_text->text,
			info->game_title_text->fontSize
		) / 2;
		
    DrawText(
        info->game_title_text->text,
        info->game_title_text->posX - x_pixels,
        info->game_title_text->posY,
        info->game_title_text->fontSize,
        info->game_title_text->color
    );

	// Draw Buttons
	DrawTexture(
			button1,
			info->button1->posX,
			info->button1->posY,
			info->button1->color
	);

    DrawTexture(
			button2,
			info->button2->posX,
			info->button2->posY,
			info->button2->color
	);

	
	// Draw score and FPS counter
	DrawFPS(SCREEN_WIDTH - 80, 0);

	EndDrawing();
}