#include "raylib.h"
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#include "state.h"
#include "interface.h"

#define STARS 200
#define SCROLL_SPEED 2

typedef struct {
    float x,    //The stars coordinates
          y,    //on the screen
          z;    //The stars depth or distance from camera
}Star;

// Assets
Texture tab_img;

Texture heart_img;
Texture empty_heart_img;

Texture jet_img;

Texture emote_fast_img;
Texture emote_neutral_img;
Texture emote_slow_img;

Texture helicopter_img;
Texture helicopter_reversed_img;
Texture warship_img;
Texture warship_reversed_img;

Texture stardust;

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
	tab_img = LoadTextureFromImage(
		LoadImage("assets/scifi_box.png")
	);

	// background_img = LoadTextureFromImage(
	// 	LoadImage("assets/space_tile.png")
	// );

	heart_img = LoadTextureFromImage(
		LoadImage("assets/heart.png")
	);

	empty_heart_img = LoadTextureFromImage(
		LoadImage("assets/empty_heart.png")
	);


	emote_fast_img = LoadTextureFromImage(
		LoadImage("assets/emote_fast.png")
	);

	emote_neutral_img = LoadTextureFromImage(
		LoadImage("assets/emote_neutral.png")
	);

	emote_slow_img = LoadTextureFromImage(
		LoadImage("assets/emote_slow.png")
	);
	
	
	jet_img = LoadTextureFromImage(
		LoadImage("assets/jet_sprite_2.png")
	);

	// jet_left_img = LoadTextureFromImage(
	// 	LoadImage("assets/jet_left.png")
	// );

	// jet_right_img = LoadTextureFromImage(
	// 	LoadImage("assets/jet_right.png")
	// );


	helicopter_img = LoadTextureFromImage(
		LoadImage("assets/helicopter.png")
	);

	helicopter_reversed_img = LoadTextureFromImage(
		LoadImage("assets/helicopter_reversed.png")
	);
	
	warship_img = LoadTextureFromImage(
		LoadImage("assets/warship.png")
	);

	warship_reversed_img = LoadTextureFromImage(
		LoadImage("assets/warship_reversed.png")
	);

	stardust = LoadTextureFromImage(
		LoadImage("assets/YellowStars1.png")
	);

	srand(time(0));
    //-----RANDOMISE THE STARS POSITIONS-----//
    for (int i = 0; i < STARS; i++) { 
        stars[i].x = GetRandomValue(0, SCREEN_W_R);
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

	float scale = 1.0f, rotation = 0.0f;
	
	// BACKGROUND
	// -------------------------------------------------------------------------------------

	// // Cleaning, draw everything from the start
	// ClearBackground(DARKBLUE);

	// const Rectangle background_rect = {0, 0, 128, 128};
	
	// // Draw Background
	// DrawTextureTiled(
    //     background_img,
    //     background_rect,
    //     (Rectangle){
    //         0.0,
    //         0.0,
    //         SCREEN_W_R,
    //         SCREEN_HEIGHT
    //     },
    //     (Vector2){0.0f, 0.0f},
    //     rotation,
    //     scale,
    //     WHITE
    // );


	ClearBackground((Color){0, 0, 0, 255});
	//-----SCROLL THE STARS-----//
        for (int i = 0; i < STARS; i++) {
            stars[i].y += SCROLL_SPEED * (stars[i].z / 1);
 
            if (stars[i].y >= SCREEN_HEIGHT) {  // Check if the star has gone off screen
                stars[i].y -= SCREEN_HEIGHT;
                stars[i].x = GetRandomValue(0, SCREEN_W_R);
            }
        }
        
        //-----DRAW THE STARS-----//
        for (int i = 0; i < STARS; i++) {
            float x = stars[i].x;
            float y = stars[i].y;
                
            DrawPixel(x, y, WHITE);
        }

	// -------------------------------------------------------------------------------------

	StateInfo info = state_info(state);

	int y_offset = info->camera_y;

	// STATUS SCREEN
	// -----------------------------------------------------------------------------------------
	// Draw Tab
	DrawTexture(
			tab_img,
			630,
			0,
			WHITE
	);

	Texture heart;
	
	//Draw 1st heart
	if (info->hearts != 0)
		heart = heart_img;
	else
		heart = empty_heart_img;

	DrawTexture(
			heart,
			SCREEN_W_R + (270 * 0.25) - 30,
			GetScreenHeight() / 2 + 150,
			WHITE
	);

	// Draw 2nd heart
	if (info->hearts >= 2)
		heart = heart_img;
	else
		heart = empty_heart_img;

	DrawTexture(
			heart,
			SCREEN_W_R + (270 * 0.5) - 30,
			GetScreenHeight() / 2 + 150,
			WHITE
	);

	// Draw 3rd heart
	if (info->hearts == 3)
		heart = heart_img;
	else
		heart = empty_heart_img;

	DrawTexture(
			heart,
			SCREEN_W_R + (270 * 0.75) - 30,
			GetScreenHeight() / 2 + 150,
			WHITE
	);

	// TEXT & EMOTE TEST
	if (keys->up) {
		DrawText(
			"SPEED: FAST",
			SCREEN_W_R + (270 / 2) - MeasureText("SPEED: FAST", 30) / 2,
			GetScreenHeight() / 2 - 50, 30, DARKGREEN
		);

		DrawTexture(
				emote_fast_img,
				SCREEN_W_R + (270 / 2) - 53,
				GetScreenHeight() / 2,
				WHITE
		);
	} else if (keys->down) {
		DrawText(
			"SPEED: SLOW",
			SCREEN_W_R + (270 / 2) - MeasureText("SPEED: SLOW", 30) / 2,
			 GetScreenHeight() / 2 - 50, 30, RED
		);

		DrawTexture(
				emote_slow_img,
				SCREEN_W_R + (270 / 2) - 53,
				GetScreenHeight() / 2,
				WHITE
		);
	} else {
		DrawText(
			"SPEED: NORMAL",
			SCREEN_W_R + (270 / 2) - MeasureText("SPEED: NORMAL", 30) / 2,
			 GetScreenHeight() / 2 - 50, 30, LIGHTGRAY
		);

		DrawTexture(
				emote_neutral_img,
				SCREEN_W_R + (270 / 2) - 53,
				GetScreenHeight() / 2,
				WHITE
		);
	}
	// -----------------------------------------------------------------------------------------

	// Camera position TEST
	DrawRectangle(
		info->camera_x,
		info->camera_y - y_offset,
		10,
		10,
		WHITE
	);

	// JET & MISSILE
	// --------------------------------------------
	// Draw jet and missile
	Color color;

	if (info->hit) {
		color = RED;
	} else {
		color = WHITE;
	}

	if (keys->left) {
		DrawTextureRec(
			jet_img,
			(Rectangle){
				5,
				0,
				35,
				43
			},
			(Vector2){
				info->jet->rect.x,
				info->jet->rect.y - y_offset
			},
			color
		);
	} else if (keys->right) {
		DrawTextureRec(
			jet_img,
			(Rectangle){
				83,
				0,
				35,
				43
			},
			(Vector2){
				info->jet->rect.x,
				info->jet->rect.y - y_offset
			},
			color
		);
	} else {
		DrawTextureRec(
			jet_img,
			(Rectangle){
				42,
				0,
				39,
				43
			},
			(Vector2){
				info->jet->rect.x,
				info->jet->rect.y - y_offset
			},
			color
		);
	}

	if (info->missile != NULL)
		DrawRectangle(
			info->missile->rect.x,
			info->missile->rect.y - y_offset,
			info->missile->rect.width,
			info->missile->rect.height,
			WHITE
		);
	// --------------------------------------------

	// OBJECTS
	// ------------------------------------------------------------
	// Create list of character that are on screen
	// Rectangle jet_rect = info->jet->rect;
	List list = state_objects(
		state,
		state->info.camera_y + SCREEN_HEIGHT,
		state->info.camera_y - SCREEN_HEIGHT
	);

	// Draw objects from list
	for	(ListNode node = list_first(list);	// iterate list
			node != LIST_EOF;
			node = list_next(list, node)) {

		Object obj = list_node_value(list, node);

		if (obj->type == TERAIN) {
			// DrawRectangle(
			// 	obj->rect.x,
			// 	obj->rect.y - y_offset,
			// 	obj->rect.width,
			// 	obj->rect.height,
			// 	GREEN
			// );
			DrawTextureTiled(
				stardust,
				(Rectangle){
					0,
					0,
					64,
					64
				},
				(Rectangle){
					obj->rect.x,
					obj->rect.y - y_offset,
					obj->rect.width,
					obj->rect.height
				},
				(Vector2){0.0f, 0.0f},
				rotation,
				scale,
				WHITE
			);
		} else if (obj->type == HELICOPTER && obj->forward) {
			DrawTexture(
				helicopter_img,
				obj->rect.x,
				obj->rect.y - y_offset,
				YELLOW
			);
		} else if (obj->type == HELICOPTER && !obj->forward) {
			DrawTexture(
				helicopter_reversed_img,
				obj->rect.x,
				obj->rect.y - y_offset,
				YELLOW
			);
		} else if (obj->type == WARSHIP && obj->forward) {
			DrawTexture(
				warship_img,
				obj->rect.x,
				obj->rect.y - y_offset,
				RED
			);
		} else if (obj->type == WARSHIP && !obj->forward) {
			DrawTexture(
				warship_reversed_img,
				obj->rect.x,
				obj->rect.y - y_offset,
				RED
			);
		} else if (obj->type == BRIDGE) {
			DrawRectangle(
				obj->rect.x,
				obj->rect.y - y_offset,
				obj->rect.width,
				obj->rect.height,
				RED
			);
		}
	}
	// ------------------------------------------------------------

	// FPS & GAME OVER TEXT
	// -----------------------------------------------------------------------------------------
	// Draw score and FPS counter
	DrawText(TextFormat("%04i", info->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	// If game is over, draw message to play again
	if (!info->playing) {
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			 GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, GRAY
		);
	}
	// -----------------------------------------------------------------------------------------

	EndDrawing();
}