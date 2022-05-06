#include "draw_related_funcs.h"
#include "start_game.h"

void draw_game(StartGame start_game, KeyState keys) {
    // get game info & tab info
	GameInfo gameinfo = start_game->game;
	// TabInfo tabinfo = start_game->tab;

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

            // If game is over, draw message to play again
			if (!gameinfo->game_state->playing) {
				DrawText(
					"PRESS [ENTER] TO PLAY AGAIN",
					SCREEN_W_G / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 30) / 2,
					SCREEN_HEIGHT / 2 - 30, 30, GRAY
				);
			}	
}

void draw_tab(StartGame start_game, KeyState keys) {
    // get game info & tab info
	GameInfo gameinfo = start_game->game;
	TabInfo tabinfo = start_game->tab;


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
			if (keys->up)
				emote = tabinfo->emote_fast_anim;
			else if (keys->down)
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
			if (keys->up)
				text = tabinfo->speed_fast_text;
			else if (keys->down)
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

			heart = start_game->tab->heart1;

			for(int i = 0 ; i < 3 ; i++){
				if (i == 0)
					heart = start_game->tab->heart1;
				else if (i == 1)
					heart = start_game->tab->heart2;
				else if (i == 2)
					heart = start_game->tab->heart3;

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
}

