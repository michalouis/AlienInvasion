#include "raylib.h"
#include "state.h"
#include "draw_related_funcs.h"
#include "game_screen.h"
#include "missile.h"
#include "enemies.h"
#include "beams.h"
#include "jet.h"
#include "game_screen_draw.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <stdio.h>
#include <stdlib.h>

Game create_game() {
    // Allocate memory for game struct in GameScreen
    Game game = malloc(sizeof(*game));

    game->playing = true;
    game->paused = false;
    game->speed_factor = 1;
    game->score = 0;
    // game->hearts = 3;
    game->missiles = set_create(missile_comparefunc, free);

    // game->hit = false;
    // game->invis_t_start = 0;

    game->jet = jet_create(SCREEN_W_G/2 - (35/2),  0, 50, 50);

    game->camera_x = SCREEN_W_G / 2;
    game->camera_y = -(SCREEN_HEIGHT / 2);

    // Δημιουργούμε τo σετ των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	game->objects = set_create(enemies_comparefunc, free);
	add_objects(game, 0);

    game->beams = set_create(missile_comparefunc, free);

    return game;
}

GameTextures create_game_textures() {
    GameTextures textures = malloc(sizeof(*textures));

    textures->jet = LoadTexture(
		"assets/jet.png"
	);

	int width = (textures->jet.width / 3) * 0.75;
	int height = textures->jet.height * 0.75;

    textures->jet_neutral_info = create_texture_info(
        (Vector2) {0, 0}, false,
        (Rectangle) {width * 1, 0, width, height},
        WHITE
    );

    textures->jet_left_info = create_texture_info(
        (Vector2) {0, 0}, false,
        (Rectangle) {width * 0, 0, width, height},
        WHITE
    );

    textures->jet_right_info = create_texture_info(
        (Vector2) {0, 0}, false,
        (Rectangle) {width * 2, 0, width, height},
        WHITE
    );

    return textures;
}

Heart create_heart(Vector2 pos, Texture texture) {
    Heart heart = malloc(sizeof(*heart));
    Rectangle rect;

    rect = (Rectangle){320, 0, 199, 199};
    heart->filled_heart = create_texture_info(pos, true, rect, WHITE);
    rect = (Rectangle){320, 199, 199, 199};
    heart->empty_heart = create_texture_info(pos, true, rect, WHITE);


    heart->heart_explode_anim = create_animation(texture, pos, 13);

    return heart;
}

void reset_heart(Heart heart) {
    Animation anim = heart->heart_explode_anim;
    animation_reset(anim);
}

Tab create_tab() {
    // Allocate memory for tab struct in GameScreen
    Tab tab = malloc(sizeof(*tab));

    // Load file with the tab assets
    tab->asset_sheet = LoadTexture(
		"assets/tabinfo_asset_sheet.png"
	);
    
    Vector2 position;   // used for position on screen
    Rectangle rect;     // used for asset position in asset_sheet

    // Create tab
    rect = (Rectangle){0, 0, 304, 576};
    position = (Vector2){SCREEN_W_G, 0};

    tab->tab_texture = create_texture_info(
        position, false,
        rect,
        WHITE
    );

    //-----Create Speed text-----//
    
    // Normal Speed
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T / 2),
        SCREEN_HEIGHT * 0.7 - 30
    };

    tab->speed_normal_text =
        create_text(
            "SPEED: NORMAL",
            position, true,
            30,
            LIGHTGRAY
        );
    
    // Slow Speed
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T / 2),
        SCREEN_HEIGHT * 0.7 - 30
    };

    tab->speed_slow_text =
        create_text(
            "SPEED: SLOW",
            position, true,
            30,
            RED
        );
    
    // Fast Speed
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T / 2),
        SCREEN_HEIGHT * 0.7 - 30
    };

    tab->speed_fast_text =
        create_text(
            "SPEED: FAST",
            position, true,
            30,
            DARKGREEN
        );

    Texture anim_texture;

    //-----Create hearts-----// - 199 / 2
    // Create heart_animation
    anim_texture = LoadTexture(
        "assets/heart_anim.png"
    );
    
    // Heart1
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T * 0.25),
        SCREEN_HEIGHT * 0.53 - 199 / 2
    };
    tab->heart1 = create_heart(position, anim_texture);

    // Heart2
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T * 0.5),
        SCREEN_HEIGHT * 0.53 - 199 / 2
    };
    tab->heart2 = create_heart(position, anim_texture);

    // Heart3
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T * 0.75),
        SCREEN_HEIGHT * 0.53 - 199 / 2
    };
    tab->heart3 = create_heart(position, anim_texture);

    //-----Create Emote animations-----//
    // Emote Neutral
    anim_texture = LoadTexture(
        "assets/emote_neutral.png"
    );

    // all emotes have the same on-screen position
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T/2) - (anim_texture.width/3) / 2,
        0
    };

    tab->emote_neutral_anim = create_animation(anim_texture, position, 3);
    
    // Emote Fast
    anim_texture = LoadTexture(
        "assets/emote_fast.png"
    );
    tab->emote_fast_anim = create_animation(anim_texture, position, 3);
    
    // Emote Slow
    anim_texture = LoadTexture(
        "assets/emote_slow.png"
    );
    tab->emote_slow_anim = create_animation(anim_texture, position, 3);

    return tab;
}

GameScreen create_game_screen() {
    // Allocate memory for GameScreen room
    GameScreen startgame = malloc(sizeof(*startgame));

    startgame->game = create_game();
    startgame->tab = create_tab();
    startgame->game_textures = create_game_textures();


    return startgame;
}

void restart_game(GameScreen game_screen) {
    Game game = game_screen->game;

	// Destroy and create new list for missiles
	set_destroy(game->missiles);	// free its memory

	// Reinitialize state information
	game->playing = true;
	game->paused = false;
	game->score = 0;
	game->missiles = set_create(missile_comparefunc, free);
	game->speed_factor = 1;

	game->jet = jet_reset(game->jet, SCREEN_W_G/2 - (35/2), 0);

	game->camera_x = SCREEN_W_G / 2;
	game->camera_y = -(SCREEN_HEIGHT / 2);

	// Destroy and create new set for objects
	set_destroy(game->objects);
	game->objects = set_create(enemies_comparefunc, free);
	add_objects(game, 0);

    Tab tab = game_screen->tab;
    reset_heart(tab->heart1);
    reset_heart(tab->heart2);
    reset_heart(tab->heart3);
}


// Finds the last bridge of the current state and returns it

Object find_last_bridge(Set set) {
	Object bridge;
	SetNode node = set_last(set);	// recover last node of set
	bridge = set_node_value(set, node);	// recover last bridge

	return bridge;
}

void game_screen_update(GameScreen game_screen, KeyState keys) {
	Game game = game_screen->game;

    //////// GAME OVER & PAUSED MODES ////////

	// If the game is over don't go deeper in the function (return)

	if (!game->playing) {
		if (keys->enter)	// If enter is pressed restart game
			restart_game(game_screen);
		
		return;
	}

    // Pause(or unpause) game if the p key is pressed

    if (keys->p)
		game->paused = !game->paused;

    // Paused mode
	// If the game is paused don't go deeper in the function to
	// prevent state from updating till the player unpauses the game

	// Debug mode (can only be accessed if game is paused)
	// The game will only move while the n key is pressed (1 frame)

	if (game->paused && !keys->n)
		return;


    game->camera_y -= 3 * game->speed_factor;

    if(IsKeyPressed(KEY_B)) {
        beam_create(game->beams, game->camera_y);
    }

    if(set_size(game->beams) != 0) {
        beam_update(game->beams);
    }

    //////// MISSILE ////////

	// The following functions handle everything that has to do with the missile

	if(keys->space)
    	missile_create(game, game->jet->rect, P_MISSILE);

	missiles_update(game);

    //////// JET ////////

	// The functions jet_movement and jet_collision handle everything that has to do with the jet

    jet_update(
        game->jet,
        game->camera_y,
        game->speed_factor,
        keys,
        game->objects
    );

	if (jet_gameover(game->jet)) {
		game->playing = false;
		return;
	}


	/////// ENEMIES ////////

	// The functions enemy_movement and enemy_collision handle everything that has to do with the enemies
	// More information about them above the state_update function (line 328 - 384)

	enemies_update(game);
    
	Set set = game->objects;

	Object last_bridge = find_last_bridge(set);
	float last_bridge_y = last_bridge->rect.y;
	if (abs(last_bridge_y - game->jet->rect.y) < SCREEN_HEIGHT) {
		add_objects(game, last_bridge_y);
		game->speed_factor += 0.3 * game->speed_factor;
	}
}

void game_screen(State state, KeyState keys) {
	if (state->game_screen == NULL)
            state->game_screen = create_game_screen();
	else
        game_screen_update(state->game_screen, keys);
}

void game_screen_draw(GameScreen game_screen, KeyState keys) {
	draw_game(game_screen, keys);
	draw_tab(game_screen, keys);
}