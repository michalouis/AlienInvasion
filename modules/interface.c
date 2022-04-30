#include "raylib.h"
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#include "state.h"
#include "interface.h"

#define STARS 200
#define SCROLL_SPEED 2


// Assets
Texture tab_img;

Texture heart_img;
Texture empty_heart_img;
Texture anim_heart_img;

Texture jet_img;

Texture emote_fast_img;
Texture emote_neutral_img;
Texture emote_slow_img;

Animation emote_neutral;
Animation heart3;

Texture helicopter_img;
Texture helicopter_reversed_img;
Texture warship_img;
Texture warship_reversed_img;

Texture stardust;

Star stars[STARS];


float randf() {
    return (rand() % 1000) / 1000.0f;
}

Animation create_animation(Texture texture, int frames, float change_frame_t) {
	Animation anim = malloc(sizeof(*anim));
	anim->texture = texture;
	anim->frameWidth = (float)(texture.width / frames);
	anim->maxFrames = (int)(texture.width / (int)anim->frameWidth);
	anim->timer = 0.0;
	anim->change_frame_t = change_frame_t;
	anim->frame = 0;
	anim->repeat = true;

	return anim;
}

void animate_loop(Animation anim, float width, float height) {
	anim->timer += GetFrameTime();

	if (anim->timer >= anim->change_frame_t) {
		anim->timer = 0.0;
		anim->frame++;
	}

	anim->frame = anim->frame % anim->maxFrames;

	Rectangle texture_rec = {
        (anim->frameWidth * anim->frame),
        0,
        anim->frameWidth,
        anim->texture.height
	};

	Vector2 vec = {width, height};

	DrawTextureRec(
        anim->texture,
        texture_rec,
        vec,
        RAYWHITE
    );
}

void animate_loop_once(Animation anim, float width, float height) {
	anim->timer += GetFrameTime();

	if (anim->timer >= anim->change_frame_t) {
		anim->timer = 0.0;
		anim->frame++;
	}

	anim->frame = anim->frame % anim->maxFrames;

	if (anim->frame == anim->maxFrames - 1) {
		anim->repeat = false;
	}

	Rectangle texture_rec = {
        (anim->frameWidth * anim->frame),
        0,
        anim->frameWidth,
        anim->texture.height
	};

	Vector2 vec = {width, height};

	DrawTextureRec(
        anim->texture,
        texture_rec,
        vec,
        RAYWHITE
    );
}

// LOAD TEXTURES
// ---------------------------------------------------------------------
void interface_init() {
	// Initialize window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

	// Load assets
	tab_img = LoadTextureFromImage(
		LoadImage("assets/metal_box.png")
	);

	// background_img = LoadTextureFromImage(
	// 	LoadImage("assets/space_tile.png")
	// );

	heart_img = LoadTextureFromImage(
		LoadImage("assets/heart_test.png")
	);

	empty_heart_img = LoadTextureFromImage(
		LoadImage("assets/empty_heart_test.png")
	);

	anim_heart_img = LoadTextureFromImage(
		LoadImage("assets/anim_heart_test.png")
	);
	heart3 = create_animation(anim_heart_img, 13, 0.04);


	emote_fast_img = LoadTextureFromImage(
		LoadImage("assets/emote_fast.png")
	);

	emote_neutral_img = LoadTextureFromImage(
		LoadImage("assets/emote_neutral.png")
	);
	emote_neutral = create_animation(emote_neutral_img, 3, 0.3);

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
			SCREEN_W_R,
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
			SCREEN_W_R + (270 * 0.25) - 30 - 94,
			GetScreenHeight() / 2 + 150 - 95,
			WHITE
	);

	// Draw 2nd heart
	if (info->hearts >= 2)
		heart = heart_img;
	else
		heart = empty_heart_img;

	DrawTexture(
			heart,
			SCREEN_W_R + (270 * 0.5) - 30 - 94,
			GetScreenHeight() / 2 + 150 - 95,
			WHITE
	);

	// Draw 3rd heart
	if (info->hearts == 3 || heart3->repeat) {
		if (info->hearts == 3) {
			DrawTexture(
				heart_img,
				SCREEN_W_R + (270 * 0.75) - 30 - 94,
				GetScreenHeight() / 2 + 150 - 95,
				WHITE
			);
		} else {
			animate_loop_once(
				heart3,
				SCREEN_W_R + (270 * 0.75) - 30 - 94,
				GetScreenHeight() / 2 + 150 - 95
			);
		}
	} else {
		DrawTexture(
			empty_heart_img,
			SCREEN_W_R + (270 * 0.75) - 30 - 94,
			GetScreenHeight() / 2 + 150 - 95,
			WHITE
		);
	}

	// DrawTexture(
	// 		heart,
	// 		SCREEN_W_R + (270 * 0.75) - 30,
	// 		GetScreenHeight() / 2 + 150,
	// 		WHITE
	// );

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

		// DrawTexture(
		// 		emote_neutral_img,
		// 		SCREEN_W_R + (270 / 2) - 53,
		// 		GetScreenHeight() / 2,
		// 		WHITE
		// );
		animate_loop(emote_neutral, SCREEN_W_R + (270 / 2) - 53, GetScreenHeight() / 2);
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