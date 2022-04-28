#include "state_update.h"

// Reinitializes state when player wants to play another round

void restart_game(State state) {
	if (state->info.missile != NULL)// if there's a missile,
		free(state->info.missile);	// free its memory

	// Reinitialize state information
	state->info.playing = true;
	state->info.paused = false;
	state->info.score = 0;
	state->info.hearts = 3;
	state->info.missile = NULL;
	state->speed_factor = 1;

	state->info.jet->rect.x = SCREEN_W_R/2 - (35/2);
	state->info.jet->rect.y = 0;

	state->info.camera_x = SCREEN_W_R / 2;
	state->info.camera_y = -(SCREEN_HEIGHT / 2);

	state->info.hit = false;
	state->info.invis_t_start = 0;

	// Destroy and create new set for objects
	set_destroy(state->objects);
	state->objects = set_create(compare_objects, free);
	add_objects(state, 0);
}


// Creates a missile if there isn't one already

void missile_fire(StateInfo info, bool key_pressed) {
	if (info->missile == NULL && key_pressed) {
		Rectangle jet_rect = info->jet->rect;	// recover jet's coordinates

		info->missile = create_object(			// create missile
			MISSLE,
			jet_rect.x + (jet_rect.width)/2,	// missile's starting position
			jet_rect.y,							// is the center of the jet's position
			5,
			15
		);
	}
}

// Moves missile depending on the game's speed

void missile_movement(Object missile, float speed) {
	if (missile != NULL)
		missile->rect.y -= 10 * speed;	// 10 pixels upwards multiplied by game's speed
}

// Checks if missile comes in contact with any objects from the list
// If it collides with a terain object, destroy the missile
// If it collides with any other object, destroy the missile, add 10 point to the score
// and remove the object from the set

void missile_collision(State state, Object missile, Set set) {
	if (missile != NULL) {
		Rectangle missile_rect = state->info.missile->rect;	// recover missile dimensions

		List list = state_objects(	//create list
			state,
			state->info.missile->rect.y + state->info.missile->rect.height,
			state->info.missile->rect.y - 4 * SPACING
		);

		for	(ListNode node = list_first(list);	// iterate list
			node != LIST_EOF;
			node = list_next(list, node)) {

			Object enemy = list_node_value(list, node);	// recover object
			Rectangle enemy_rect = enemy->rect;			// recover object dimensions

			bool collision = CheckCollisionRecs(	// does the missile collide with this object?
				missile_rect, enemy_rect
			);
					
			if (collision) {	// if they collide, go in
				if (enemy->type == TERAIN) {
					free(state->info.missile);	// destroy missile
					state->info.missile = NULL;
				} else {
					state->info.score += 10;	// increase score
					set_remove(set, enemy);		// remove object
					free(state->info.missile);	// destroy missile
					state->info.missile = NULL;
				}
				list_destroy(list);	//free list memory
				return;
			}
		}

		list_destroy(list);	//free list memory
	}
}

// Checks if the missile is too far away from the jet 
// and destroys it if it is

void missile_destroy(StateInfo info) {
	if (info->missile != NULL) {
		float jet_y = info->jet->rect.y;	// recover jet's y coordinate
		float missile_y = info->missile->rect.y;	// recover missile's y coordinate
			
		if (abs(missile_y - jet_y) > SCREEN_HEIGHT) {
			free(info->missile);	// destroy missile
			info->missile = NULL;
		}
	}
}


// Moves jet depending on the pressed keys
// and the game's speed

void jet_movement(Object jet, float speed, KeyState keys) {
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
}

// Checks if jet comes in contact with any objects from the list
// If it does it returns "true", else it returns "false"

bool jet_collision(State state, Rectangle jet_rect) {
	bool collision = false;

	List list = state_objects(	//create list
		state,
		state->info.jet->rect.y + state->info.jet->rect.height,
		state->info.jet->rect.y - 4 * SPACING
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


// Checks if an enemy is about to collide with a terain object 
// If it does change its direction, else don't

void enemy_collision(Object enemy, Set set) {

	// Find the first terain object after the enemy
	Object obj;
	SetNode node = set_find_node(set, enemy);
	do {
		node = set_next(set, node);
		obj = set_node_value(set, node);
	} while (obj->type != TERAIN);

	// Objects are stored in the set in such way that
	// the next node contains the other terain object
	Object terain_1 = set_node_value(set, node);
	Object terain_2 = set_node_value(set, set_next(set, node));

	// Find which terain object is the left
	// one and which is the right one
	Object terain_left;
	Object terain_right;
	if (terain_1->rect.x < terain_2->rect.x) {
		terain_left = terain_1;
		terain_right = terain_2;
	} else {
		terain_left = terain_2;
		terain_right = terain_1;
	}

	// Find x coordinates the enemy can move between
	// before touching a terain object 
	float x_left = terain_left->rect.width;
	float x_right = terain_right->rect.x;

	// If enemy is about to collide with a terain object change its direction
	if (enemy->rect.x < x_left || enemy->rect.x + enemy->rect.width > x_right)
		enemy->forward = !enemy->forward;

}

// Move enemies depending on the the direction
// they are facing and on the game's speed

void enemy_movement(Object enemy, float speed) {
	int pixels;
	if (enemy->type == HELICOPTER)	// enemies move differently
		pixels = 4;	// helictopers move 4 pixels
	else
		pixels = 3;	// warships move 3 pixels

	if (enemy->forward)	// depending on their direction
						// they move left or right
		enemy->rect.x += pixels;	// move "pixels" right multiplied by game's speed
	else
		enemy->rect.x -= pixels;	// move "pixels" left multiplied by game's speed
}


// Finds the last bridge of the current state and returns it

Object find_last_bridge(Set set) {
	Object bridge;
	SetNode node = set_last(set);	// recover last node of set
	bridge = set_node_value(set, node);	// recover last bridge

	return bridge;
}