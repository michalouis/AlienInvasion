#pragma once

#include "raylib.h"
// #include "interface.h"
#include "ADTSet.h"

#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576
#define SCREEN_W_R 720
#define SCREEN_W_T 304

typedef enum {
	TITLE_SCREEN, START_GAME, CHARACTER_SELECT
} StateName;

typedef enum {
	TERAIN, HELICOPTER, WARSHIP, JET, MISSLE, BRIDGE
} ObjectType;

typedef struct text_info {
	const char* text;
	Vector2 pos;
	int fontSize;
	Color color;
}* TextInfo;

typedef struct texture_info {
	Vector2 pos;
	Rectangle rect;
	Color color;
}* TextureInfo;

typedef struct animation_info {
	Vector2 pos;
	float frameWidth;
	int maxFrames;
	float timer;
	float change_frame_t;
	int curr_frame;
}* AnimationInfo;

typedef struct animation {
    Texture texture;
    AnimationInfo info;
}* Animation;

typedef struct button {
	TextureInfo texture_info;
	bool pressed;
}* Button;

typedef struct title_screen {
	TextInfo title_text;
	Texture asset_sheet;
	int button_selected;
	Button button1;
	Button button2;
}* TitleScreen;

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

// typedef struct char_select {

// }* CharSelect;

typedef struct state_info {
	TitleScreen title_screen;
	StartGame start_game;
	// struct char_select char_select;
}* StateInfo;

typedef struct state {
	StateName name;
	struct state_info info;
}* State;

// Πληροφορίες για το ποια πλήκτρα είναι πατημένα
typedef struct key_state {
	bool up;
	bool down;
	bool left;
	bool right;
	bool space;
	bool enter;
	bool n;
	bool p;
}* KeyState;

// STATE_CREATE

State state_create();

// STATE_INFO

void* state_info(State state);

// STATE_UPDATE

void state_update(State state, KeyState keys);

void state_destroy(State state);

void destroy_title_scr(State state);
