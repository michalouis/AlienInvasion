#include "draw_related_funcs.h"
#include "missile.h"
#include "enemies.h"
#include "jet.h"
#include "start_game.h"

void draw_game(StartGame start_game, KeyState keys) {
    // get game info & tab info
	Game game = start_game->game;
	GameTextures game_textures= start_game->game_textures;

    // Draw Jet
			Color color;

			if (game->jet->hit) {
				color = RED;
			} else {
				color = WHITE;
			}

			TextureInfo jet_texture_info;
			if(keys->left)
				jet_texture_info = game_textures->jet_left_info;
			else if(keys->right)
				jet_texture_info = game_textures->jet_right_info;
			else
				jet_texture_info = game_textures->jet_neutral_info;

			DrawRectangle(
				// gameinfo->game_textures->jet,
				// (Vector2) {
					game->jet->rect.x,
					game->jet->rect.y - game->camera_y,
				// },
				jet_texture_info->rect.width,
				jet_texture_info->rect.height,
				color
			);

			// Draw Missile
			if (game->missiles != 0)
				for(SetNode node = set_first(game->missiles);
					node != SET_EOF;
					node = set_next(game->missiles, node)) {
				
					Missile missile = set_node_value(game->missiles, node);
					DrawRectangle(
						missile->rect.x,
						missile->rect.y - game->camera_y,
						missile->rect.width,
						missile->rect.height,
						missile->type == P_MISSILE ? RED : BLUE
					);
				}

			// OBJECTS
			// ------------------------------------------------------------
			// Create list of character that are on screen
			// Rectangle jet_rect = info->jet->rect;
			List list = state_enemies(
				game->objects,
				game->camera_y + SCREEN_HEIGHT,
				game->camera_y - SCREEN_HEIGHT
			);

			// Draw objects from list
			for	(ListNode node = list_first(list);	// iterate list
					node != LIST_EOF;
					node = list_next(list, node)) {

				Object obj = list_node_value(list, node);
				
				DrawRectangle(
					obj->rect.x,
					obj->rect.y - game->camera_y,
					obj->rect.width,
					obj->rect.height,
					GREEN
				);
			}
			// ------------------------------------------------------------

            // If game is over, draw message to play again
			if (!game->playing) {
				DrawText(
					"PRESS [ENTER] TO PLAY AGAIN",
					SCREEN_W_G / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 30) / 2,
					SCREEN_HEIGHT / 2 - 30, 30, GRAY
				);
			}	
}

void draw_tab(StartGame start_game, KeyState keys) {
    // get game info & tab info
	Game game = start_game->game;
	Tab tab = start_game->tab;


    // Draw Tab
			
			// DrawTextureRec(
			// 	tab->asset_sheet,
			// 	tab->tab_texture->rect,
			// 	(Vector2) {
			// 		tab->tab_texture->pos.x,
			// 		tab->tab_texture->pos.y,
			// 	},
			// 	tab->tab_texture->color
			// );
			DrawRectangle(
				tab->tab_texture->pos.x,
				tab->tab_texture->pos.y,
				SCREEN_W_T,
				SCREEN_HEIGHT,
				MAROON
			);

			// Emote
			Animation emote;
			if (keys->up)
				emote = tab->emote_fast_anim;
			else if (keys->down)
				emote = tab->emote_slow_anim;
			else
				emote = tab->emote_neutral_anim;

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
				text = tab->speed_fast_text;
			else if (keys->down)
				text = tab->speed_slow_text;
			else
				text = tab->speed_normal_text;

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

				if (game->jet->hearts <= i) {
					bool active = animate(
						heart->heart_explode_anim,
						(Vector2) {
							heart->filled_heart->pos.x,
							heart->filled_heart->pos.y
						},
						0.04,
						false
					);

					if(!active) {
						heart_texture = heart->empty_heart;

						DrawTextureRec(
							tab->asset_sheet,
							heart_texture->rect,
							(Vector2) {
								heart_texture->pos.x,
								heart_texture->pos.y
							},
							heart_texture->color
						);
					}
				} else {
						
					heart_texture = heart->filled_heart;

					DrawTextureRec(
						tab->asset_sheet,
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
				TextFormat("SCORE: %04i", game->score),
				SCREEN_W_T / 2 + SCREEN_W_G - MeasureText(TextFormat("SCORE: %04i", game->score), 30) / 2,
				SCREEN_HEIGHT * 0.75 - 30, 30, GRAY
			);
}

