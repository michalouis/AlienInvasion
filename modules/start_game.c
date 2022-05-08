#include "raylib.h"
#include "state.h"
#include "draw_related_funcs.h"
#include "start_game.h"
#include "missile.h"
#include "enemies.h"
#include "start_game_draw.h"
#include "set_utils.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <stdio.h>
#include <stdlib.h>

Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;

	if (type == WARSHIP || type == HELICOPTER){
		obj->timer = time(NULL);
		obj->countdown = time(NULL) + GetRandomValue(2,3);
	}

	return obj;
}

void add_objects(GameState gamestate, float start_y) {
	// Προσθέτουμε BRIDGE_NUM γέφυρες.
	// Στο διάστημα ανάμεσα σε δύο διαδοχικές γέφυρες προσθέτουμε:
	// - Εδαφος, αριστερά και δεξιά της οθόνης (με μεταβαλλόμενο πλάτος).
	// - 3 εχθρούς (ελικόπτερα και πλοία)
	// Τα αντικείμενα έχουν SPACING pixels απόσταση μεταξύ τους.

	for (int i = 0; i < BRIDGE_NUM; i++) {
		// Δημιουργία γέφυρας
		Object bridge = create_object(
			BRIDGE,
			0,								// x στο αριστερό άκρο της οθόνης
			start_y - 4 * (i+1) * SPACING,	// Η γέφυρα i έχει y = 4 * (i+1) * SPACING
			SCREEN_W_G,						// Πλάτος ολόκληρη η οθόνη
			20								// Υψος
		);

		// // Δημιουργία εδάφους
		// Object terain_left = create_object(
		// 	TERAIN,
		// 	0,								// Αριστερό έδαφος, x = 0
		// 	bridge->rect.y,					// y ίδιο με την γέφυρα
		// 	rand() % (SCREEN_W_G/3),		// Πλάτος επιλεγμένο τυχαία
		// 	4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		// );
		// int width = rand() % (SCREEN_W_G/2);
		// Object terain_right = create_object(
		// 	TERAIN,
		// 	SCREEN_W_G - width,			// Δεξί έδαφος, x = <οθόνη> - <πλάτος εδάφους>
		// 	bridge->rect.y,					// y ίδιο με τη γέφυρα
		// 	width,							// Πλάτος, επιλεγμένο τυχαία
		// 	4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		// );

		// set_insert(state->objects, terain_left);
		// set_insert(state->objects, terain_right);
		set_insert(gamestate->objects, bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_W_G - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_W_G - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			set_insert(gamestate->objects, enemy);
		}
	}
}

// CompareFunc comparing objects (used for set)

int compare_objects(Pointer a, Pointer b) {
	Object obj_a = a;
	Object obj_b = b;

	// comparing coordinates of y axis
	if (obj_a->rect.y < obj_b->rect.y) {
		return 1;
	} else if (obj_a->rect.y > obj_b->rect.y) {
		return -1;
	} else {
		
		// // Special case: bridge asset won't be placed above terain
		// if (obj_a->type == TERAIN && obj_b->type == BRIDGE) {
		// 	return 1;
		// }
		// if(obj_a->type == BRIDGE && obj_b->type == TERAIN) {
		// 	return -1;
		// }

		// comparing memory addresses
		if (a < b) {
			return 1;
		} else if (a > b) {
			return -1;
		} else {
			return 0;
		}
	}
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη x είναι ανάμεσα στο x_from και x_to.

List state_objects(GameState gamestate, float y_from, float y_to) {
	List list = list_create(NULL);	// create list


	Object obj1 = malloc(sizeof(Object));	// allocate memory
	obj1->rect.y = y_from;	// give y coordinate to obj1 
							// (required for the next functions)

	Object obj2 = malloc(sizeof(Object));	// allocate memory
	obj2->rect.y = y_to;	// give y coordinate to obj2
							// (required for the next functions)

	Set set = gamestate->objects;

	// Find first and last objects between y_from and y_to coordinates
	Object obj_first = set_find_eq_or_greater(set, obj1);
	Object obj_last = set_find_eq_or_smaller(set, obj2);

	// If there are no objects between y_from and y_to,
	// return empty list
	if (obj_first == NULL || obj_last == NULL) {
		return list;
	}

	// Iterate set from obj_first to obj_last and add
	// objects to the list we created

	SetNode first_node = set_find_node(set, obj_first);
	SetNode last_node = set_find_node(set, obj_last);
	
	for(SetNode node = first_node;
		node != set_next(set, last_node);
		node = set_next(set, node)) {

		Object obj = set_node_value(set, node);			// recover object
		list_insert_next(list, list_last(list), obj);	// insert object to list
	}

	return list;
}

GameState create_gameinfo_state() {
    // Allocate memory for game struct in StartGame
    GameState gamestate = malloc(sizeof(*gamestate));

    gamestate->playing = true;
    gamestate->paused = false;
    gamestate->speed_factor = 1;
    gamestate->score = 0;
    gamestate->hearts = 3;
    gamestate->missiles = set_create(compare_objects, free);

    gamestate->hit = false;
    gamestate->invis_t_start = 0;

    gamestate->jet = create_object(JET, SCREEN_W_G/2 - (35/2),  0, 50, 50);

    gamestate->camera_x = SCREEN_W_G / 2;
    gamestate->camera_y = -(SCREEN_HEIGHT / 2);

    // Δημιουργούμε τo σετ των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	gamestate->objects = set_create(compare_objects, free);
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
	gamestate->hearts = 3;
	gamestate->missiles = set_create(compare_objects, free);
	gamestate->speed_factor = 1;

	gamestate->jet->rect.x = SCREEN_W_G/2 - (35/2);
	gamestate->jet->rect.y = 0;

	gamestate->camera_x = SCREEN_W_G / 2;
	gamestate->camera_y = -(SCREEN_HEIGHT / 2);

	gamestate->hit = false;
	gamestate->invis_t_start = 0;

	// Destroy and create new set for objects
	set_destroy(gamestate->objects);
	gamestate->objects = set_create(compare_objects, free);
	add_objects(gamestate, 0);
}


// Moves jet depending on the pressed keys
// and the game's speed

void jet_movement(GameState gamestate, float speed, KeyState keys) {
    Object jet = gamestate->jet;
    // float camera_x = gamestate->camera_x;
    float camera_y = gamestate->camera_y;

	if (keys->up)
		jet->rect.y -= 7 * speed;	// 6 pixels upwards multiplied by game's speed
	else if (keys->down)
		jet->rect.y += 1.5 * speed;	// 2 pixels upwards multiplied by game's speed
	else
		jet->rect.y -= 3 * speed;	// 3 pixels upwards multiplied by game's speed

	if (keys->left && keys->right)	// If both left and right arrows are
		return;						// pressed don't move left or right
	else if (keys->left)
		jet->rect.x -= 3 * speed;	// 3 pixels left multiplied by game's speed
	else if (keys->right)
		jet->rect.x += 3 * speed;	// 3 pixels right multiplied by game's speed
    
    if (jet->rect.y < camera_y + 10)
        jet->rect.y = camera_y + 10;

    if (jet->rect.y > camera_y + SCREEN_HEIGHT - jet->rect.height - 10)
        jet->rect.y = camera_y + SCREEN_HEIGHT - jet->rect.height - 10;

    if (jet->rect.x < 10)
        jet->rect.x = 10;
    
    if (jet->rect.x > SCREEN_W_G - jet->rect.width - 10)
        jet->rect.x = SCREEN_W_G - jet->rect.width - 10;
}

// Checks if jet comes in contact with any objects from the list
// If it does it returns "true", else it returns "false"

bool jet_collision(GameState gamestate, Rectangle jet_rect) {
	bool collision = false;

	List list = state_objects(	//create list
		gamestate,
		gamestate->jet->rect.y + gamestate->jet->rect.height,
		gamestate->jet->rect.y - 4 * SPACING
	);

	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Object obj = list_node_value(list, node);	// recover object
		collision = CheckCollisionRecs(		// check if jet and object collide
			jet_rect, obj->rect
		);

		if (collision) {
			list_destroy(list);	//free list memory
			return true;		// if they collide, return "true"
		}	
	}

	list_destroy(list);		// free list memory
	return false;			// if none of the objects of the list
							// collide with the jet, return "false"
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
    	missile_create(gamestate, gamestate->jet, P_MISSILE);

	printf("MPAINO\n");
	missiles_update(gamestate);

    //////// JET ////////

	// The functions jet_movement and jet_collision handle everything that has to do with the jet

    jet_movement(gamestate, gamestate->speed_factor, keys);

    // if (!gamestate->hit) {
	// 	if (jet_collision(gamestate, gamestate->jet->rect)) {	// If the jet collides with 
	// 		gamestate->hearts--;											// an object, stop the game
	// 		gamestate->hit = true;
	// 		gamestate->invis_t_start = time(NULL);
	// 	}
	// }

	if (!gamestate->hearts) {
		gamestate->playing = false;
		return;
	}

	if (gamestate->hit) {
		time_t t_now = time(NULL);

		if(t_now > gamestate->invis_t_start + 5) {
			gamestate->invis_t_start = 0;
			gamestate->hit = false;
		}
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