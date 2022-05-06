#pragma once

#include "state.h"
#include "ADTList.h"

#define BRIDGE_NUM 100		// πόσες γέφυρες δημιουργούνται στην πίστα
#define SPACING 200			// απόσταση ανάμεσα στα αντικείμενα της πίστας

typedef enum {
	TERAIN, HELICOPTER, WARSHIP, JET, MISSLE, BRIDGE
} ObjectType;

typedef struct heart {
	TextureInfo filled_heart;
	Animation heart_explode_anim;
}* Heart;

typedef struct tab_info {
	Texture asset_sheet;
	TextureInfo tab_texture;
	Heart heart1;
	Heart heart2;
	Heart heart3;
	Animation emote_neutral_anim;
	Animation emote_fast_anim;
	Animation emote_slow_anim;
	TextInfo speed_normal_text;
	TextInfo speed_fast_text;
	TextInfo speed_slow_text;

}* TabInfo;

// Πληροφορίες για κάθε αντικείμενο
typedef struct object {
	ObjectType type;				// Τύπος (Εδαφος / Ελικόπτερο / Πλοίο / Αεροσκάφος / Πύραυλος / Γέφυρα)
	Rectangle rect;					// Θέση και μέγεθος του αντικειμένου. Το Rectangle ορίζεται στο include/raylib.h, line 213
	bool forward;					// true: το αντικείμενο κινείται προς τα δεξιά
}* Object;

typedef struct game_state {
	Object jet;
	Object missile;
	Set objects;
	bool playing;
	bool paused;
	float speed_factor;
	int score;
	int hearts;
	bool hit;
	time_t invis_t_start;
	float camera_x;
	float camera_y;
}* GameState;

typedef struct game_textures {
	Texture jet;
	TextureInfo jet_neutral_info;
	TextureInfo jet_left_info;
	TextureInfo jet_right_info;
}* GameTextures;

typedef struct game_info {
	GameState game_state;
	GameTextures game_textures;
}* GameInfo;

typedef struct start_game {
	GameInfo game;
	TabInfo tab;

}* StartGame;

List state_objects(GameState gamestate, float y_from, float y_to);

void start_game(State state, KeyState keys);

void start_game_draw(StartGame start_game, KeyState keys);