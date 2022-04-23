#include "raylib.h"
#include <stdio.h>

#include "state.h"
#include "interface.h"

// Assets
Texture jet_neutral_img;
Texture jet_left_img;
Texture jet_right_img;
Texture helicopter_img;
Texture helicopter_reversed_img;
Texture warship_img;
Texture warship_reversed_img;
Texture tab_img;


void interface_init() {
	// Initialize window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

	// Load assets
	jet_neutral_img = LoadTextureFromImage(
		LoadImage("assets/jet_neutral.png")
	);

	jet_left_img = LoadTextureFromImage(
		LoadImage("assets/jet_left.png")
	);

	jet_right_img = LoadTextureFromImage(
		LoadImage("assets/jet_right.png")
	);

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

	tab_img = LoadTextureFromImage(
		LoadImage("assets/tab.png")
	);
}

void interface_close() {
	CloseWindow();
}

// Draw game (one frame)
void interface_draw_frame(State state, KeyState keys) {
	BeginDrawing();

	// Cleaning, draw everything from the start
	ClearBackground(SKYBLUE);

	StateInfo info = state_info(state);

	int y_offset = info->jet->rect.y - 600;

	// Draw Tab
	DrawTexture(
			tab_img,
			630,
			0,
			WHITE
	);

	// TEXT TEST
	if (keys->up) {
		DrawText(
			"SPEED: FAST",
			SCREEN_W_R + (270 / 2) - MeasureText("SPEED: FAST", 30) / 2,
			 GetScreenHeight() / 2 - 50, 30, DARKGREEN
		);
	} else if (keys->down) {
		DrawText(
			"SPEED: SLOW",
			SCREEN_W_R + (270 / 2) - MeasureText("SPEED: SLOW", 30) / 2,
			 GetScreenHeight() / 2 - 50, 30, RED
		);
	} else {
		DrawText(
			"SPEED: NORMAL",
			SCREEN_W_R + (270 / 2) - MeasureText("SPEED: NORMAL", 30) / 2,
			 GetScreenHeight() / 2 - 50, 30, LIGHTGRAY
		);
	}

	// Draw jet and missile
	if (keys->left) {
		DrawTexture(
			jet_left_img,
			info->jet->rect.x,
			info->jet->rect.y - y_offset,
			WHITE
		);
	} else if (keys->right) {
		DrawTexture(
			jet_right_img,
			info->jet->rect.x,
			info->jet->rect.y - y_offset,
			WHITE
		);
	} else {
		DrawTexture(
			jet_neutral_img,
			info->jet->rect.x,
			info->jet->rect.y - y_offset,
			WHITE
		);
	}

	if (info->missile != NULL)
		DrawRectangle(
			info->missile->rect.x,
			info->missile->rect.y - y_offset,
			info->missile->rect.width,
			info->missile->rect.height,
			BLACK
		);

	// Create list of character that are on screen
	Rectangle jet_rect = info->jet->rect;
	List list = state_objects(
		state,
		jet_rect.y + SCREEN_HEIGHT,
		jet_rect.y - 2 * SCREEN_HEIGHT
	);

	// Draw objects from list
	for	(ListNode node = list_first(list);	// iterate list
			node != LIST_EOF;
			node = list_next(list, node)) {

		Object obj = list_node_value(list, node);

		if (obj->type == TERAIN) {
			DrawRectangle(
				obj->rect.x,
				obj->rect.y - y_offset,
				obj->rect.width,
				obj->rect.height,
				GREEN
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

	EndDrawing();
}