#pragma once

#include "state.h"
#include "jet.h"
#include "ADTList.h"

#define BRIDGE_NUM 50		// πόσες γέφυρες δημιουργούνται στην πίστα
#define SPACING 200			// απόσταση ανάμεσα στα αντικείμενα της πίστας

typedef enum {
	TERAIN, HELICOPTER, WARSHIP, JET, MISSLE, BRIDGE
} ObjectType;

typedef struct heart {
	TextureInfo filled_heart;
	TextureInfo empty_heart;
	Animation heart_explode_anim;
}* Heart;

typedef struct tab {
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

}* Tab;

// Πληροφορίες για κάθε αντικείμενο
typedef struct object {
	ObjectType type;				// Τύπος (Εδαφος / Ελικόπτερο / Πλοίο / Αεροσκάφος / Πύραυλος / Γέφυρα)
	Rectangle rect;					// Θέση και μέγεθος του αντικειμένου. Το Rectangle ορίζεται στο include/raylib.h, line 213
	time_t countdown;
	time_t timer;
	bool forward;					// true: το αντικείμενο κινείται προς τα δεξιά
}* Object;

typedef struct game {
	Jet jet;
	Set missiles;
	Set objects;
	bool playing;
	bool paused;
	float speed_factor;
	int score;
	float camera_x;
	float camera_y;
}* Game;

typedef struct game_textures {
	Texture jet;
	TextureInfo jet_neutral_info;
	TextureInfo jet_left_info;
	TextureInfo jet_right_info;
}* GameTextures;

typedef struct game_screen {
	Game game;
	Tab tab;
	GameTextures game_textures;
}* GameScreen;

// List state_objects(Game, float y_from, float y_to);

void game_screen(State state, KeyState keys);

void game_screen_draw(GameScreen game_screen, KeyState keys);