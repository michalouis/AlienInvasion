#include "game_screen.h"
#include "draw_related_funcs.h"
#include "raylib.h"
#include "state.h"
#include "missile.h"
#include "enemies.h"
#include "beams.h"
#include "jet.h"
#include "ADTSet.h"

#include <stdlib.h>

extern Music music;

// Create game struct

static Game create_game() {
    // Allocate memory for game struct
    Game game = malloc(sizeof(*game));

    // Initialize game status
    game->playing = true;
    game->paused = false;
    game->score = 0;
    game->speed_factor = 1;
    game->difficulty = 1;
    game->difficulty_changed = false;

    // create jet
    game->jet = jet_create(SCREEN_W_G/2 - (35/2),  200, 42, 42);

    // create missile set
    game->missiles = set_create(missile_comparefunc, free);
    
    // create enemies set
	game->enemies = set_create(enemies_comparefunc, free);
	add_enemies(game, 0); // add enemies

    // create beams set
    game->beams = set_create(missile_comparefunc, free);
    game->beam_cooldown = 20;    // beams wont appear in the game immediately

    // starting camera position
    game->camera_y = -(SCREEN_HEIGHT / 2);

    // Play music
    PlayMusicStream(music);

    return game;
}

// Load assets

static GameAssets create_game_assets() {
    GameAssets assets = malloc(sizeof(*assets));

    ///// TEXTURES /////
    assets->jet = LoadTexture("assets/game_assets/jet.png");
    assets->shield = LoadTexture("assets/game_assets/shield.png");
    assets->p_missile = LoadTexture("assets/game_assets/p_missile.png");
    assets->e_missile = LoadTexture("assets/game_assets/e_missile.png");

    ///// TEXTURE INFO /////
	int width = (assets->jet.width / 3);
	int height = assets->jet.height;

    // jet neutral
    assets->jet_neutral_info = textureInfo_create(
        (Vector2) {0, 0}, false,
        (Rectangle) {width * 0, 0, width, height},
        WHITE
    );

    // jet left
    assets->jet_left_info = textureInfo_create(
        (Vector2) {0, 0}, false,
        (Rectangle) {width * 2, 0, width, height},
        WHITE
    );

    // jet right
    assets->jet_right_info = textureInfo_create(
        (Vector2) {0, 0}, false,
        (Rectangle) {width * 1, 0, width, height},
        WHITE
    );

    ///// ANIMATIONS /////
    Texture anim;

    // crab
    anim = LoadTexture("assets/game_assets/animation_crab.png");
    assets->anim_crab = animation_create(anim, 2);

    // longhorn
    anim = LoadTexture("assets/game_assets/animation_longhorn.png");
    assets->anim_longhorn = animation_create(anim, 2);

    // mothership
    anim = LoadTexture("assets/game_assets/animation_mothership.png");
    assets->anim_mothership = animation_create(anim, 6);

    // mothership defender
    anim = LoadTexture("assets/game_assets/animation_mothership_defender_right.png");
    assets->anim_mothership_defender_right = animation_create(anim, 2);

    anim = LoadTexture("assets/game_assets/animation_mothership_defender_left.png");
    assets->anim_mothership_defender_left = animation_create(anim, 2);

    // beam
    anim = LoadTexture("assets/game_assets/animation_beam.png");
    assets->anim_beam = animation_create(anim, 4);

    // warning sign
    anim = LoadTexture("assets/game_assets/animation_warning.png");
    assets->anim_warning_sign = animation_create(anim, 2);

    // pause text
    anim = LoadTexture("assets/game_assets/animation_paused_text.png");
    assets->anim_pause_text = animation_create(anim, 4);
    
    // explosion
    anim = LoadTexture("assets/game_assets/animation_explosion.png");
    assets->anim_explosion = animation_create(anim, 3);

    // gameover text
    anim = LoadTexture("assets/game_assets/animation_gameover_text.png");
    assets->anim_gameover_text = animation_create(anim, 4);

    // revealing & disappearing bar
    anim = LoadTexture("assets/game_assets/animation_revealing_bar.png");
    assets->anim_reaviling_bar = animation_create(anim, 4);

    anim = LoadTexture("assets/game_assets/animation_disappearing_bar.png");
    assets->anim_disappearing_bar = animation_create(anim, 4);

    // speedup text
    anim = LoadTexture("assets/game_assets/animation_speedup.png");
    assets->anim_speedup = animation_create(anim, 5);

    

    ///// SOUNDS /////

    // player missile
    assets->sound_p_missile = LoadSound("assets/game_assets/player_missile.mp3");
    SetSoundVolume(assets->sound_p_missile, 0.6);

    // shield
    assets->sound_shield = LoadSound("assets/game_assets/shield.mp3");
    SetSoundVolume(assets->sound_shield, 0.3);

    // out of shield energy
    assets->sound_no_shield = LoadSound("assets/game_assets/noshield.mp3");
    SetSoundVolume(assets->sound_no_shield, 0.6);

    // warning
    assets->sound_warning = LoadSound("assets/game_assets/warning.mp3");

    // score reward
    assets->sound_score_reward = LoadSound("assets/game_assets/score_reward.mp3");

    // hit sounds
    assets->sound_hit_player = LoadSound("assets/game_assets/player_hit.mp3");
    SetSoundVolume(assets->sound_hit_player, 0.8);

    assets->sound_hit_beam = LoadSound("assets/game_assets/beam_hit.mp3");

    assets->sound_hit_enemy = LoadSound("assets/game_assets/enemy_hit.mp3");
    SetSoundVolume(assets->sound_hit_enemy, 0.6);
    
    // explosion
    assets->sound_explosion = LoadSound("assets/game_assets/explosion.mp3");

    return assets;
}

// Creates heart struct to store heart assets/animations related to the heart

static Heart create_heart(Vector2 pos, Texture texture) {
    Heart heart = malloc(sizeof(*heart));
    Rectangle rect;

    // filled heart texture
    rect = (Rectangle){0, 0, 199, 199};
    heart->filled_heart = textureInfo_create(pos, true, rect, WHITE);

    // empty heart texture
    rect = (Rectangle){0, 199, 199, 199};
    heart->empty_heart = textureInfo_create(pos, true, rect, WHITE);
    
    // heart explosion animation
    heart->anim_heart_explosion = animation_create(texture, 13);

    return heart;
}

static void destroy_heart(Heart heart) {
    textureInfo_destroy(heart->empty_heart);
    textureInfo_destroy(heart->filled_heart);
    animation_destroy(heart->anim_heart_explosion);

    free(heart);
}

// Resets heart animation

static void reset_heart_animation(Heart heart) {
    Animation anim = heart->anim_heart_explosion;
    animation_reset(anim);
}

// Create tab struct

static Tab create_tab() {
    // Allocate memory for tab struct
    Tab tab = malloc(sizeof(*tab));

    ///// TEXTURES /////
    tab->tab_texture = LoadTexture("assets/tab/tab.png");
    tab->score = LoadTexture("assets/tab/score_tab.png");
    tab->missile = LoadTexture("assets/tab/missile_tab.png");
    tab->shield = LoadTexture("assets/tab/shield_tab.png");
    tab->heart_sprites = LoadTexture("assets/tab/hearts.png");
    tab->bar = LoadTexture("assets/tab/bar.png");

    ///// ANIMATIONS /////
    Texture anim_texture;

    // Bar animation
    anim_texture = LoadTexture("assets/tab/animation_bar.png");
    tab->anim_bar = animation_create(anim_texture, 4);

    //-----Create Emote animations-----//

    // Emote neutral
    anim_texture = LoadTexture("assets/tab/emote_neutral.png");
    tab->emote_neutral = animation_create(anim_texture, 3);
    
    // Emote hit
    anim_texture = LoadTexture("assets/tab/emote_hit.png");
    tab->emote_hit = animation_create(anim_texture, 6);
    
    // Emote gameover
    anim_texture = LoadTexture("assets/tab/emote_gameover.png");
    tab->emote_gameover = animation_create(anim_texture, 3);
    
    //-----Create Gameover animation-----//

    anim_texture = LoadTexture("assets/tab/animation_tv_static.png");
    tab->anim_tv_static = animation_create(anim_texture, 6);

    ///// SOUNDS /////
    tab->sound_tv_static = LoadSound("assets/tab/tv_static.mp3");

    ///// ETC /////
    
    //-----Create hearts-----//
    anim_texture = LoadTexture("assets/tab/animation_heart_explosion.png");

    float j = 0.25, k = 0.61;   // used for heart position
    for(int i = 0; i < 6; i++) {
        if (j > 0.75) j = 0.25;
        if (i > 2) k = 0.7;

        Vector2 position = (Vector2){
            SCREEN_W_G + (SCREEN_W_T * j),
            SCREEN_HEIGHT * k - 199 / 2
        };
        tab->hearts[i] = create_heart(position, anim_texture);

        j += 0.25;
    }

    return tab;
}

GameScreen create_game_screen() {
    // Allocate memory for GameScreen room
    GameScreen startgame = malloc(sizeof(*startgame));

    startgame->game = create_game();
    startgame->tab = create_tab();
    startgame->game_assets = create_game_assets();


    return startgame;
}

void restart_game(GameScreen game_screen) {
    Game game = game_screen->game;

	// Reinitialize state information
	game->playing = true;
	game->paused = false;
    game->score = 0;
	game->speed_factor = 1;
    game->difficulty = 1;
    game->difficulty_changed = false;
	
    // reset jet position
	game->jet = jet_reset(game->jet, SCREEN_W_G/2 - (35/2), 200);

    // Destroy and create new set for missiles
	set_destroy(game->missiles);
	game->missiles = set_create(missile_comparefunc, free);
	// Destroy and create new set for enemies
	set_destroy(game->enemies);
	game->enemies = set_create(enemies_comparefunc, free);
    add_enemies(game, 0);
	// Destroy and create new set for beams
	set_destroy(game->beams);
    game->beams = set_create(missile_comparefunc, free);
    game->beam_cooldown = 20; 

    //reset camera position
	game->camera_y = -(SCREEN_HEIGHT / 2);

    // Reset animations
    animation_reset(game_screen->game_assets->anim_explosion);

    Tab tab = game_screen->tab;
    for(int i = 0; i < 6; i++)
        reset_heart_animation(tab->hearts[i]);


    // Replay music from the start
    PlayMusicStream(music);
}

static void game_screen_update(GameScreen game_screen, KeyState keys) {
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

	if (game->paused)
		return;

    // Increase difficulty
    if (game->score > 1200 && game->difficulty == 1) {
        game->speed_factor = 1.25;
		game->difficulty = 2;
        game->difficulty_changed = true;
    } else if (game->score > 2300 && game->difficulty == 2) {
        game->speed_factor = 1.5;
		game->difficulty = 3;
        game->difficulty_changed = true;
    } else if (game->score > 3500 && game->difficulty == 3) {
        game->speed_factor = 1.55;
		game->difficulty = 4;
        game->difficulty_changed = true;
    }

    // Update camera position
    game->camera_y -= 3 * game->speed_factor;

    // Spawn beam randomly when cooldown reaches 0
    game->beam_cooldown -= GetFrameTime();
    if(game->beam_cooldown <= 0 && set_size(game->beams) <= 3) {
        beam_create(game->beams, jet_position(game->jet));

        if (game->difficulty == 1)
            game->beam_cooldown = 10;
        else if (game->difficulty == 4)
            game->beam_cooldown = 5;
        else
            game->beam_cooldown = 7;
    }

    // Update beams
    if(set_size(game->beams) != 0) {
        beam_update(game->beams, game->speed_factor);
    }

    // create missile if space is pressed
	if (keys->space && !game->jet->hit_by_beam && game->jet->missiles > 0) {
    	missile_create(game, game->jet->rect, P_MISSILE);
        PlaySound(game_screen->game_assets->sound_p_missile);
    }

    // update missiles
	missiles_update(game, game_screen->game_assets);

    // enable/disable shield if X is pressed
    if (keys->x && !game->jet->shield_cooldown) {
        game->jet->shield = !game->jet->shield;
        PlaySound(game_screen->game_assets->sound_shield);
    } else if (keys->x && game->jet->shield_cooldown){
        PlaySound(game_screen->game_assets->sound_no_shield);
    }

    // update jet
    jet_update(
        game->jet,
        game->camera_y,
        game->speed_factor,
        keys,
        game->enemies,
        game->beams,
        game_screen->game_assets->sound_hit_player,
        game_screen->game_assets->sound_hit_beam
    );

    // update bar

    if (game->jet->bar == 30) {
        if (game->jet->hearts != 6) {
            reset_heart_animation(
                game_screen->tab->hearts[game->jet->hearts]
            );
            game->jet->hearts++;
        } else {
            game->score += 200;
        }

        PlaySound(game_screen->game_assets->sound_score_reward);
        game->jet->bar = 0;
    }

    // if game is over the make jet explode and stop the game
	if (jet_gameover(game->jet)) {
		game->playing = false;
        PlaySound(game_screen->game_assets->sound_explosion);
		return;
	}

    // update enemies
	enemies_update(game);
}

void game_screen(State state, KeyState keys) {
	if (state->game_screen == NULL)
            state->game_screen = create_game_screen();
	else
        game_screen_update(state->game_screen, keys);

        if ((state->game_screen->game->paused && keys->b) ||
            (!state->game_screen->game->playing && keys->b)) {
            destroy_game_screen(state);
            state->name = TITLE_SCREEN;
        }
}

void destroy_game_screen(State state) {
    GameScreen game_screen = state->game_screen;  

    // Destroy game
    Game game = game_screen->game;

    jet_destroy(game->jet);
    set_destroy(game->missiles);
    set_destroy(game->enemies);
    set_destroy(game->beams);

    // Destroy game assets
    GameAssets game_assets = game_screen->game_assets;

    UnloadTexture(game_assets->jet);
    UnloadTexture(game_assets->shield);
    UnloadTexture(game_assets->p_missile);
    UnloadTexture(game_assets->e_missile);

    textureInfo_destroy(game_assets->jet_neutral_info);
    textureInfo_destroy(game_assets->jet_left_info);
    textureInfo_destroy(game_assets->jet_right_info);

    animation_destroy(game_assets->anim_crab);
    animation_destroy(game_assets->anim_longhorn);
    animation_destroy(game_assets->anim_mothership);
    animation_destroy(game_assets->anim_mothership_defender_right);
    animation_destroy(game_assets->anim_mothership_defender_left);
    animation_destroy(game_assets->anim_beam);
    animation_destroy(game_assets->anim_warning_sign);
    animation_destroy(game_assets->anim_pause_text);
    animation_destroy(game_assets->anim_explosion);
    animation_destroy(game_assets->anim_gameover_text);
    animation_destroy(game_assets->anim_reaviling_bar);
    animation_destroy(game_assets->anim_disappearing_bar);
    animation_destroy(game_assets->anim_speedup);

    UnloadSound(game_assets->sound_p_missile);
    UnloadSound(game_assets->sound_shield);
    UnloadSound(game_assets->sound_no_shield);
    UnloadSound(game_assets->sound_warning);
    UnloadSound(game_assets->sound_score_reward);
    UnloadSound(game_assets->sound_hit_player);
    UnloadSound(game_assets->sound_hit_beam);
    UnloadSound(game_assets->sound_hit_enemy);
    UnloadSound(game_assets->sound_explosion);

    // Destroy tab
    Tab tab = game_screen->tab;

    UnloadTexture(tab->tab_texture);
    UnloadTexture(tab->score);
    UnloadTexture(tab->missile);
    UnloadTexture(tab->shield);
    UnloadTexture(tab->heart_sprites);
    UnloadTexture(tab->bar);

    animation_destroy(tab->emote_neutral);
    animation_destroy(tab->emote_hit);
    animation_destroy(tab->emote_gameover);
    animation_destroy(tab->anim_tv_static);
    animation_destroy(tab->anim_bar);

    UnloadSound(tab->sound_tv_static);

    for(int i = 0; i < 6; i++) {
        destroy_heart(tab->hearts[i]);
    }


    free(game_screen);

    state->game_screen = NULL;

    StopMusicStream(music);
}