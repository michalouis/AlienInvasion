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
			// get startgame info
			StartGame info = state->info.start_game;
			
			// get game info & tab info
			GameInfo gameinfo = info->game;
			TabInfo tabinfo = info->tab;

			// Draw Jet
			Color color;

			if (gameinfo->game_state->hit) {
				color = RED;
			} else {
				color = WHITE;
			}

			TextureInfo jet_texture_info;
			if(keys->left)
				jet_texture_info = gameinfo->game_textures->jet_left_info;
			else if(keys->right)
				jet_texture_info = gameinfo->game_textures->jet_right_info;
			else
				jet_texture_info = gameinfo->game_textures->jet_neutral_info;

			DrawTextureRec(
				gameinfo->game_textures->jet,
				jet_texture_info->rect,
				(Vector2) {
					gameinfo->game_state->jet->rect.x,
					gameinfo->game_state->jet->rect.y - gameinfo->game_state->camera_y
				},
				color
			);

			// Draw Missile
			if (gameinfo->game_state->missile != NULL)
				DrawRectangle(
					gameinfo->game_state->missile->rect.x,
					gameinfo->game_state->missile->rect.y - gameinfo->game_state->camera_y,
					gameinfo->game_state->missile->rect.width,
					gameinfo->game_state->missile->rect.height,
					WHITE
				);

			// OBJECTS
			// ------------------------------------------------------------
			// Create list of character that are on screen
			// Rectangle jet_rect = info->jet->rect;
			List list = state_objects(
				gameinfo->game_state,
				gameinfo->game_state->camera_y + SCREEN_HEIGHT,
				gameinfo->game_state->camera_y - SCREEN_HEIGHT
			);

			// Draw objects from list
			for	(ListNode node = list_first(list);	// iterate list
					node != LIST_EOF;
					node = list_next(list, node)) {

				Object obj = list_node_value(list, node);
				
				DrawRectangle(
					obj->rect.x,
					obj->rect.y - gameinfo->game_state->camera_y,
					obj->rect.width,
					obj->rect.height,
					GREEN
				);
			}
			// ------------------------------------------------------------

			// Draw Tab
			// DrawTextureRec(
			// 	tabinfo->asset_sheet,
			// 	tabinfo->tab_texture->rect,
			// 	(Vector2) {
			// 		tabinfo->tab_texture->pos.x,
			// 		tabinfo->tab_texture->pos.y,
			// 	},
			// 	tabinfo->tab_texture->color
			// );
			DrawRectangle(
				tabinfo->tab_texture->pos.x,
				tabinfo->tab_texture->pos.y,
				SCREEN_W_T,
				SCREEN_HEIGHT,
				MAROON
			);

			// Emote
			Animation emote;
			if (IsKeyDown(KEY_UP))
				emote = tabinfo->emote_fast_anim;
			else if (IsKeyDown(KEY_DOWN))
				emote = tabinfo->emote_slow_anim;
			else
				emote = tabinfo->emote_neutral_anim;

			DrawRectangle(
				emote->info->pos.x,
				emote->info->pos.y + 10,
				254,
				254,
				BLACK
			);

			animate(
				emote,
				(Vector2) {
					emote->info->pos.x,
					emote->info->pos.y
				},
				0.2,
				true
			);

			// Speed text
			TextInfo text;
			if (IsKeyDown(KEY_UP))
				text = tabinfo->speed_fast_text;
			else if (IsKeyDown(KEY_DOWN))
				text = tabinfo->speed_slow_text;
			else
				text = tabinfo->speed_normal_text;

			DrawText(
				text->text,
				text->pos.x,
				text->pos.y,
				text->fontSize,
				text->color
			);

			// Hearts
			Heart heart;
			TextureInfo heart_texture;

			heart = info->tab->heart1;

			for(int i = 0 ; i < 3 ; i++){
				if (i == 0)
					heart = info->tab->heart1;
				else if (i == 1)
					heart = info->tab->heart2;
				else if (i == 2)
					heart = info->tab->heart3;

				if (gameinfo->game_state->hearts <= i) {
					animate(
						heart->heart_explode_anim,
						(Vector2) {
							heart->filled_heart->pos.x,
							heart->filled_heart->pos.y
						},
						0.04,
						false
					);
				} else {
						// if (gameinfo->hearts <= i)
						// 	heart_texture = heart->empty_heart;
						// else
						heart_texture = heart->filled_heart;

					DrawTextureRec(
						tabinfo->asset_sheet,
						heart_texture->rect,
						(Vector2) {
							heart_texture->pos.x,
							heart_texture->pos.y
						},
						heart_texture->color
					);
				}
			}

			// Draw score
			DrawText(
				TextFormat("SCORE: %04i", gameinfo->game_state->score),
				SCREEN_W_T / 2 + SCREEN_W_G - MeasureText(TextFormat("SCORE: %04i", gameinfo->game_state->score), 30) / 2,
				SCREEN_HEIGHT * 0.75 - 30, 30, GRAY
			);

			// If game is over, draw message to play again
			if (!gameinfo->game_state->playing) {
				DrawText(
					"PRESS [ENTER] TO PLAY AGAIN",
					SCREEN_W_G / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 30) / 2,
					SCREEN_HEIGHT / 2 - 30, 30, GRAY
				);
			}

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