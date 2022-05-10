#include "raylib.h"
#include "state.h"
#include "draw_related_funcs.h"
#include "start_game.h"
#include "missile.h"
#include "enemies.h"
#include "jet.h"
#include "start_game_draw.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <stdio.h>
#include <stdlib.h>

GameState create_gameinfo_state() {
    // Allocate memory for game struct in StartGame
    GameState gamestate = malloc(sizeof(*gamestate));

    gamestate->playing = true;
    gamestate->paused = false;
    gamestate->speed_factor = 1;
    gamestate->score = 0;
    // gamestate->hearts = 3;
    gamestate->missiles = set_create(missile_comparefunc, free);

    // gamestate->hit = false;
    // gamestate->invis_t_start = 0;

    gamestate->jet = jet_create(SCREEN_W_G/2 - (35/2),  0, 50, 50);

    gamestate->camera_x = SCREEN_W_G / 2;
    gamestate->camera_y = -(SCREEN_HEIGHT / 2);

    // Δημιουργούμε τo σετ των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	gamestate->objects = set_create(enemies_comparefunc, free);
	add_objects(gamestate, 0);

    return gamestate;
}

GameTextures create_game_textures() {
    GameTextures textures = malloc(sizeof(*textures));

    textures->jet = LoadTexture(
		"assets/jet.png"
	);

	int width = textures->jet.width / 3;
	int height = textures->jet.height;

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

GameInfo create_gameinfo() {
    GameInfo gameinfo = malloc(sizeof(*gameinfo));

    gameinfo->game_state = create_gameinfo_state();
    gameinfo->game_textures = create_game_textures();

    return gameinfo;
}

Heart create_heart(Vector2 pos, Texture texture) {
    Heart heart = malloc(sizeof(*heart));
    Rectangle rect;

    rect = (Rectangle){320, 0, 199, 199};
    heart->filled_heart = create_texture_info(pos, true, rect, WHITE);

    heart->heart_explode_anim = create_animation(texture, pos, 13);

    return heart;
}

TabInfo create_tabinfo() {
    // Allocate memory for tab struct in StartGame
    TabInfo tabinfo = malloc(sizeof(*tabinfo));

    // Load file with the tab assets
    tabinfo->asset_sheet = LoadTexture(
		"assets/tabinfo_asset_sheet.png"
	);
    
    Vector2 position;   // used for position on screen
    Rectangle rect;     // used for asset position in asset_sheet

    // Create tab
    rect = (Rectangle){0, 0, 304, 576};
    position = (Vector2){SCREEN_W_G, 0};

    tabinfo->tab_texture = create_texture_info(
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

    tabinfo->speed_normal_text =
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

    tabinfo->speed_slow_text =
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

    tabinfo->speed_fast_text =
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
    tabinfo->heart1 = create_heart(position, anim_texture);

    // Heart2
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T * 0.5),
        SCREEN_HEIGHT * 0.53 - 199 / 2
    };
    tabinfo->heart2 = create_heart(position, anim_texture);

    // Heart3
    position = (Vector2){
        SCREEN_W_G + (SCREEN_W_T * 0.75),
        SCREEN_HEIGHT * 0.53 - 199 / 2
    };
    tabinfo->heart3 = create_heart(position, anim_texture);

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

    tabinfo->emote_neutral_anim = create_animation(anim_texture, position, 3);
    
    // Emote Fast
    anim_texture = LoadTexture(
        "assets/emote_fast.png"
    );
    tabinfo->emote_fast_anim = create_animation(anim_texture, position, 3);
    
    // Emote Slow
    anim_texture = LoadTexture(
        "assets/emote_slow.png"
    );
    tabinfo->emote_slow_anim = create_animation(anim_texture, position, 3);

    return tabinfo;
}

StartGame create_start_game() {
    // Allocate memory for StartGame room
    StartGame startgame = malloc(sizeof(*startgame));

    startgame->game = create_gameinfo();
    startgame->tab = create_tabinfo();

    return startgame;
}

void restart_game(GameState gamestate) {
	// Destroy and create new list for missiles
	set_destroy(gamestate->missiles);	// free its memory

	// Reinitialize state information
	gamestate->playing = true;
	gamestate->paused = false;
	gamestate->score = 0;
	// gamestate->hearts = 3;
	gamestate->missiles = set_create(missile_comparefunc, free);
	gamestate->speed_factor = 1;

	gamestate->jet = jet_reset(gamestate->jet, SCREEN_W_G/2 - (35/2), 0);

	gamestate->camera_x = SCREEN_W_G / 2;
	gamestate->camera_y = -(SCREEN_HEIGHT / 2);

	// gamestate->hit = false;
	// gamestate->invis_t_start = 0;

	// Destroy and create new set for objects
	set_destroy(gamestate->objects);
	gamestate->objects = set_create(enemies_comparefunc, free);
	add_objects(gamestate, 0);
}


// Finds the last bridge of the current state and returns it

Object find_last_bridge(Set set) {
	Object bridge;
	SetNode node = set_last(set);	// recover last node of set
	bridge = set_node_value(set, node);	// recover last bridge

	return bridge;
}

void start_game_update(StartGame info, KeyState keys) {
	GameState gamestate = info->game->game_state;

    //////// GAME OVER & PAUSED MODES ////////

	// If the game is over don't go deeper in the function (return)

	if (!gamestate->playing) {
		if (keys->enter)	// If enter is pressed restart game
			restart_game(gamestate);
		
		return;
	}

    // Pause(or unpause) game if the p key is pressed

    if (keys->p)
		gamestate->paused = !gamestate->paused;

    // Paused mode
	// If the game is paused don't go deeper in the function to
	// prevent state from updating till the player unpauses the game

	// Debug mode (can only be accessed if game is paused)
	// The game will only move while the n key is pressed (1 frame)

	if (gamestate->paused && !keys->n)
		return;


    gamestate->camera_y -= 3 * gamestate->speed_factor;

    //////// MISSILE ////////

	// The following functions handle everything that has to do with the missile

	if(keys->space)
    	missile_create(gamestate, gamestate->jet->rect, P_MISSILE);

	missiles_update(gamestate);

    //////// JET ////////

	// The functions jet_movement and jet_collision handle everything that has to do with the jet

    jet_update(
        gamestate->jet,
        keys,
        gamestate->speed_factor,
        gamestate->camera_y,
        gamestate->objects
    );

	if (jet_gameover(gamestate->jet)) {
		gamestate->playing = false;
		return;
	}


	/////// ENEMIES ////////

	// The functions enemy_movement and enemy_collision handle everything that has to do with the enemies
	// More information about them above the state_update function (line 328 - 384)

	enemies_update(gamestate);
    
	Set set = gamestate->objects;

	Object last_bridge = find_last_bridge(set);
	float last_bridge_y = last_bridge->rect.y;
	if (abs(last_bridge_y - gamestate->jet->rect.y) < SCREEN_HEIGHT) {
		add_objects(gamestate, last_bridge_y);
		gamestate->speed_factor += 0.3 * gamestate->speed_factor;
	}
}

void start_game(State state, KeyState keys) {
	if (state->start_game == NULL)
            state->start_game = create_start_game();
	else
        start_game_update(state->start_game, keys);
}

void start_game_draw(StartGame start_game, KeyState keys) {
	draw_game(start_game, keys);
	draw_tab(start_game, keys);
}