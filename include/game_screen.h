#pragma once

#include "state.h"
#include "jet.h"
#include "ADTList.h"

#define ENEMY_ROWS 20		// πόσες γέφυρες δημιουργούνται στην πίστα
#define SPACING 150			// απόσταση ανάμεσα στα αντικείμενα της πίστας

typedef struct game {
	// Primary game status
	bool playing;
	bool paused;
	int score;
	float speed_factor;
	int difficulty;
	float camera_y;

	// Secondary game status
	float beam_cooldown;
	bool difficulty_changed;

	//Objects
	Jet jet;
	Set missiles;
	Set enemies;
	Set beams;
}* Game;

typedef struct game_assets {
	// Textures //
	Texture jet;
	Texture shield;
	Texture mothership_defender;
	Texture p_missile;
	Texture e_missile;

	// Texture Info //
	TextureInfo jet_neutral_info;
	TextureInfo jet_left_info;
	TextureInfo jet_right_info;

	// Animations //
	Animation anim_crab;
	Animation anim_longhorn;
	Animation anim_mothership;
	Animation anim_mothership_defender_right;
	Animation anim_mothership_defender_left;
	Animation anim_beam;
	Animation anim_warning_sign;
	Animation anim_pause_text;
	Animation anim_explosion;
	Animation anim_gameover_text;
	Animation anim_reaviling_bar;
	Animation anim_disappearing_bar;
	Animation anim_speedup;

	// Sounds //
	Sound sound_p_missile;
	Sound sound_shield;
	Sound sound_no_shield;
	Sound sound_warning;
	Sound sound_score_reward;
	Sound sound_hit_player;
	Sound sound_hit_beam;
	Sound sound_hit_enemy;
	Sound sound_explosion;
}* GameAssets;

typedef struct heart {
	TextureInfo filled_heart;
	TextureInfo empty_heart;
	Animation anim_heart_explosion;
}* Heart;

typedef struct tab {
	// Textures //
	Texture tab_texture;
	Texture score;
	Texture missile;
	Texture shield;
	Texture heart_sprites;
	Texture bar;

	// Animations //
	Animation emote_neutral;
	Animation emote_hit;
	Animation emote_gameover;
	Animation anim_tv_static;
	Animation anim_bar;

	// Sounds //
	Sound sound_tv_static;

	// Etc //
	Heart hearts[6];
}* Tab;

typedef struct game_screen {
	Game game;
	GameAssets game_assets;
	Tab tab;
}* GameScreen;

void game_screen(State state, KeyState keys);

void destroy_game_screen(State state);