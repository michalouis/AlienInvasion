#include "game_screen.h"
#include "game_screen_draw.h"
#include "draw_related_funcs.h"
#include "missile.h"
#include "enemies.h"
#include "beams.h"
#include "jet.h"

#include <math.h>

extern Music music;

static void draw_game(GameScreen game_screen, KeyState keys) {
    // get game info & tab info
	Game game = game_screen->game;
	GameAssets game_assets= game_screen->game_assets;

	// Stop music when game is over
	if (!game->playing) {
		StopMusicStream(music);
	}

	// Draw Missiles
	if (set_size(game->missiles) != 0) {
		for(SetNode node = set_first(game->missiles);
			node != SET_EOF;
			node = set_next(game->missiles, node)) {
				
			Missile missile = set_node_value(game->missiles, node);

			if (missile->type != P_MISSILE)
				DrawTexture (
					game_assets->e_missile,
					missile->rect.x,
					missile->rect.y - game->camera_y,
					RAYWHITE
				);
			else
				DrawTexture (
					game_assets->p_missile,
					missile->rect.x,
					missile->rect.y - game->camera_y,
					RAYWHITE
				);
		}
	}

	// Draw enemies
	// Create list of enemies that are on screen
	// Rectangle jet_rect = info->jet->rect;
	List list = state_enemies(
		game->enemies,
		game->camera_y + SCREEN_HEIGHT + SPACING,
		game->camera_y - SCREEN_HEIGHT - SPACING
	);

	// Draw enemies from list
	for	(ListNode node = list_first(list);	// iterate list
			node != LIST_EOF;
			node = list_next(list, node)) {

			Enemy enemy = list_node_value(list, node);

			switch (enemy->type) {
				case MOTHERSHIP:
					DrawTexture(
						game_assets->mothership,
						enemy->right ? enemy->rect.x - 21 : enemy->rect.x,
						enemy->rect.y - game->camera_y - 55,
						PURPLE
					);
					break;
				case CRAB:
					animation_animate(
						game_assets->anim_crab,
						(Vector2) {
							enemy->rect.x,
							enemy->rect.y - game->camera_y
						},
						1, YELLOW, true
					);
					break;
				case LONGHORN:
					animation_animate(
						game_assets->anim_longhorn,
						(Vector2) {
							enemy->rect.x,
							enemy->rect.y - game->camera_y
						},
						1, BEIGE, true
					);
					break;
				case MOTHERSHIP_DEFENDER:
					DrawTextureRec(
						game_assets->mothership_defender,
						(Rectangle) {
							enemy->right ? 0 : enemy->rect.width, 0,
							enemy->rect.width, enemy->rect.height
						},
						(Vector2) {
							enemy->rect.x,
							enemy->rect.y - game->camera_y
						},
						SKYBLUE
					);
					break;
			}			
	}

	list_destroy(list);

    // Draw Jet
	// Choose jet color
	Color color;
	if (game->jet->hit)
		color = RED;
	else if (game->jet->hit_by_beam)
		color = GREEN;
	else
		color = WHITE;

	// Choose jet texture
	TextureInfo jet_texture_info;
	if(keys->left)
		jet_texture_info = game_assets->jet_left_info;
	else if(keys->right)
		jet_texture_info = game_assets->jet_right_info;
	else
		jet_texture_info = game_assets->jet_neutral_info;

	if (game->playing) {
		// if game is playing draw jet
		DrawTextureRec(
			game_assets->jet,
			jet_texture_info->rect,
			(Vector2) {
				game->jet->rect.x - 2,
				game->jet->rect.y - 2 - game->camera_y
			},
			color
		);

		// Draw bar of effect near jet
		if (game->jet->hit_by_beam)
			DrawRectangle(
				game->jet->rect.x + game->jet->rect.width + 5,
				game->jet->rect.y - game->camera_y,
				25 * (game->jet->effect_time / 8), 3, GREEN
			);

		if (game->jet->hit)
			DrawRectangle(
				game->jet->rect.x + game->jet->rect.width + 5,
				game->jet->rect.y - game->camera_y,
				25 * (game->jet->invisibility_time / 5), 3, RED
			);
	} else {
		// if game is over play explosion animation
		if (!animation_finished(game_assets->anim_explosion, 0.5))
			animation_animate(
				game_assets->anim_explosion,
				(Vector2) {
					game->jet->rect.x - 4,
					game->jet->rect.y - 4 - game->camera_y
				},
				0.5, RAYWHITE, false
			);
	}

	// Draw shield around jet, if it is enabled
	if (game->jet->shield) {
		DrawTexture(
			game_assets->shield,
			game->jet->rect.x + game->jet->rect.width / 2 - game_assets->shield.width / 2,
			game->jet->rect.y + game->jet->rect.width / 2 - game_assets->shield.height / 2 - game->camera_y,
			RAYWHITE
		);
	}


	// Draw beams
	if(set_size(game->beams) != 0) {
		for(SetNode node = set_first(game->beams);
			node != SET_EOF;
			node = set_next(game->beams, node)) {
				
			Beam beam = set_node_value(game->beams, node);
					
			Vector2 vec;
			vec.x = beam->rect.x;
			vec.y = beam->rect.y - game->camera_y;
			animation_animate(game_assets->anim_beam, vec, 0.3, RAYWHITE, true);
		}
	}

	// Draw warning signs
	if (set_size(game->beams) != 0 && game->playing) {
		for(SetNode node = set_first(game->beams);
			node != SET_EOF;
			node = set_next(game->beams, node)) {

			Beam beam = set_node_value(game->beams, node);
			if (beam->rect.x + beam->rect.width < 0 ||
				beam->rect.x > SCREEN_W_G) {

				if(!IsSoundPlaying(game_assets->sound_warning)) {
					PlaySound(game_assets->sound_warning);
				}

				animation_animate(
					game_assets->anim_warning_sign,
					(Vector2) {
						beam->move_right ? 10 : (SCREEN_W_G - 50) - 10,
						beam->rect.y - game->camera_y
					},
					0.13, RAYWHITE, true);
			}
		}
	}

    // If game is over, draw message to play again
	if (!game->playing) {
		animation_animate(
			game_assets->anim_gameover_text,
			(Vector2) {
				SCREEN_W_G / 2 - game_assets->anim_gameover_text->frameWidth / 2,
				SCREEN_HEIGHT / 2 - game_assets->anim_gameover_text->texture.height / 2
			},
			0.75, RAYWHITE, true
		);
				
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			SCREEN_W_G / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 30) / 2,
			SCREEN_HEIGHT * 0.6, 30, GRAY
		);

		DrawText(
			"OR PRESS [B] TO GO TO THE TITLE SCREEN",
			SCREEN_W_G / 2 - MeasureText("OR PRESS [B] TO GO TO THE TITLE SCREEN", 30) / 2,
			SCREEN_HEIGHT * 0.65, 30, GRAY
		);
	}

	// if game is paused, draw message and cover the screen to appear blur
	if (game->paused) {
		DrawRectangle(
			0, 0,
			SCREEN_W_G,
			SCREEN_HEIGHT,
			(Color){ 200, 200, 200, 100 }
		);

		animation_animate(
			game_assets->anim_pause_text,
			(Vector2) {
				SCREEN_W_G / 2 - game_assets->anim_pause_text->frameWidth / 2,
				SCREEN_HEIGHT / 2 - game_assets->anim_pause_text->texture.height / 2
			},
			0.75, RAYWHITE, true
		);

		DrawText(
			"PRESS [B] TO GO TO THE TITLE SCREEN",
			SCREEN_W_G / 2 - MeasureText("PRESS [B] TO GO TO THE TITLE SCREEN", 30) / 2,
			SCREEN_HEIGHT * 0.6, 30, BLACK
		);
	}
}

static void draw_tab(GameScreen game_screen, KeyState keys) {
    // get game info & tab info
	Game game = game_screen->game;
	Tab tab = game_screen->tab;


    // Draw Tab
	DrawTexture(
		tab->tab_texture,
		SCREEN_W_G,
		0,
		RAYWHITE
	);

	// Draw Emote

	// What emote animation will be used
	Animation emote;
	if (!game_screen->game->playing) {
		emote = tab->emote_gameover;
	} else if (game_screen->game->jet->hit) {
		if (!animation_finished(tab->emote_hit, 0.10))
			emote = tab->emote_hit;
		else
			emote = tab->emote_neutral;
	} else {
		if (animation_finished(tab->emote_hit, 0.10))
			animation_reset(tab->emote_hit);
				
		if (animation_finished(tab->emote_gameover, 0.10))
			animation_reset(tab->emote_gameover);

		emote = tab->emote_neutral;
	}

	// emote position
	Vector2 position = {
		SCREEN_W_G + (SCREEN_W_T/2) - (emote->frameWidth) / 2,
		20
	};

	if (game_screen->game->playing) {
		animation_animate(
			emote,
			position,
			0.10,
			RAYWHITE, true
		);

		if (IsSoundPlaying(tab->sound_tv_static))
			StopSound(tab->sound_tv_static);
	} else {
		// if game is over play gameover emote and then play tv static
		if (animation_finished(emote, 0.1)) {
			if (!IsSoundPlaying(tab->sound_tv_static))
				PlaySound(tab->sound_tv_static);
			
			animation_animate(
				tab->anim_tv_static,
				position,
				0.1,
				RAYWHITE, true
			);
		} else {
			animation_animate(
				emote,
				position,
				0.1, RAYWHITE, false
			);
		}
	}

	// Draw Hearts
	TextureInfo heart_texture;
	Heart *hearts = tab->hearts;

	for(int i = 0 ; i < 6 ; i++){
		if (game->jet->hearts <= i) {
			// if player loses heart play explosion animation
			bool active = animation_animate(
				hearts[i]->anim_heart_explosion,
				(Vector2) {
					hearts[i]->filled_heart->pos.x,
					hearts[i]->filled_heart->pos.y
				},
				0.04, RAYWHITE, false
			);

			// after animation is finished draw empty heart
			if(!active) {
				heart_texture = hearts[i]->empty_heart;

				DrawTextureRec(
					tab->heart_sprites,
					heart_texture->rect,
					(Vector2) {
						heart_texture->pos.x,
						heart_texture->pos.y
					},
					heart_texture->color
				);
			}
		} else {
			// if player hasn't lost heart draw filled heart		
			heart_texture = hearts[i]->filled_heart;
			DrawTextureRec(
				tab->heart_sprites,
				heart_texture->rect,
				(Vector2) {
					heart_texture->pos.x,
					heart_texture->pos.y
				},
				heart_texture->color
			);
		}
	}

	// Draw Missile

	// Draw missile text
	float pos_x = SCREEN_W_G + 29;
	DrawTextureRec(
		tab->missile,
		(Rectangle) {0, 0, 169, 33},
		(Vector2) {pos_x, SCREEN_HEIGHT * 0.77} , RAYWHITE
	);

	// draw remaning missiles
	pos_x += 179;
	for (int i = 0; i < game->jet->missiles; i++) {
		DrawTextureRec(
			tab->missile,
			(Rectangle) {0, 33, 14, 32},
			(Vector2) {pos_x, SCREEN_HEIGHT * 0.77} , RAYWHITE
		);
		pos_x += 30;
	}

	// draw used missiles
	for (int i = 0; i < 5 - game->jet->missiles; i++) {
		DrawTextureRec(
			tab->missile,
			(Rectangle) {14, 33, 14, 32},
			(Vector2) {pos_x, SCREEN_HEIGHT * 0.77} , RAYWHITE
		);
		pos_x += 30;
	}

	// Draw Shield

	// Draw shield text
	pos_x = SCREEN_W_G + 29;
	DrawTextureRec(
		tab->shield,
		(Rectangle) {0, 0, 169, 33},
		(Vector2) {pos_x, SCREEN_HEIGHT * 0.83} , RAYWHITE
	);

	// Draw shield energy bar
	pos_x += 179;
	DrawTextureRec(
		tab->shield,
		(Rectangle) {0, 33, 164, 32},
		(Vector2) {pos_x, SCREEN_HEIGHT * 0.83} , RAYWHITE
	);

	// Draw how much shield energy is left
	pos_x = SCREEN_W_G + 29 + 179 + 4;
	DrawRectangle(
		pos_x, SCREEN_HEIGHT * 0.83 + 4,
		156 * (game->jet->shield_time / 5), 20,
		game->jet->shield_cooldown ? RED : (Color) {91, 110, 225, 255}
	);

	// Draw score

	// Draw score text
	pos_x = SCREEN_W_G + 29;
	DrawTextureRec(
		tab->score,
		(Rectangle) {0, 0, 169, 33},
		(Vector2) {pos_x, SCREEN_HEIGHT * 0.89} , RAYWHITE
	);

	// Draw score from textures
	pos_x += 179;
	int score = game->score;
	for (int i = 0; i < 5; i++) {

		int j = (int) (pow(10, (4 - i)));
		if (j == 0) j = 1;
		int n = score / j;
		DrawTextureRec(
			tab->score,
			(Rectangle) {n*28, 33, 28, 32},
			(Vector2) {pos_x + 5*i, SCREEN_HEIGHT * 0.89} , RAYWHITE
		);

		score = score % j;
		pos_x += 28 + 1;
	}
}

void game_screen_draw(GameScreen game_screen, KeyState keys) {
	draw_game(game_screen, keys);
	draw_tab(game_screen, keys);
}