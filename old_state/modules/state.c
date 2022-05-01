#include <stdlib.h>

#include "ADTList.h"
#include "state.h"
#include "raylib.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	List objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από y = start_y, και επεκτείνονται προς τα πάνω.
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα
// καρτεσιανό επίπεδο.
// - Στο άξονα x το 0 είναι το αριστερό μέρος της πίστας και οι συντεταγμένες
//   μεγαλώνουν προς τα δεξιά. Η πίστα έχει σταθερό πλάτος, οπότε όλες οι
//   συντεταγμένες x είναι ανάμεσα στο 0 και το SCREEN_WIDTH.
//
// - Στον άξονα y το 0 είναι η αρχή της πίστας, και οι συντεταγμένες μεγαλώνουν
//   προς τα _κάτω_. Η πίστα αντιθέτως επεκτείνεται προς τα πάνω, οπότε όλες οι
//   συντεταγμένες των αντικειμένων είναι _αρνητικές_.
//
// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
// στο include/raylib.h). Τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle.

static void add_objects(State state, float start_y) {
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
			SCREEN_WIDTH,					// Πλάτος ολόκληρη η οθόνη
			20								// Υψος
		);

		// Δημιουργία εδάφους
		Object terain_left = create_object(
			TERAIN,
			0,								// Αριστερό έδαφος, x = 0
			bridge->rect.y,					// y ίδιο με την γέφυρα
			rand() % (SCREEN_WIDTH/4),		// Πλάτος επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);
		int width = rand() % (SCREEN_WIDTH/3);
		Object terain_right = create_object(
			TERAIN,
			SCREEN_WIDTH - width,			// Δεξί έδαφος, x = <οθόνη> - <πλάτος εδάφους>
			bridge->rect.y,					// y ίδιο με τη γέφυρα
			width,							// Πλάτος, επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);

		list_insert_next(state->objects, list_last(state->objects), terain_left);
		list_insert_next(state->objects, list_last(state->objects), terain_right);
		list_insert_next(state->objects, list_last(state->objects), bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_WIDTH - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_WIDTH - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			list_insert_next(state->objects, list_last(state->objects), enemy);
		}
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->info.score = 0;					// Αρχικό σκορ 0
	state->info.missile = NULL;				// Αρχικά δεν υπάρχει πύραυλος
	state->speed_factor = 1;				// Κανονική ταχύτητα

	// Δημιουργία του αεροσκάφους, κεντραρισμένο οριζόντια και με y = 0
	state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);

	// Δημιουργούμε τη λίστα των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	state->objects = list_create(free);
	add_objects(state, 0);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη y είναι ανάμεσα στο y_from και y_to.

List state_objects(State state, float y_from, float y_to) {
	List list = list_create(NULL);		// create list

	for	(ListNode node = list_first(state->objects);		// iteration of state->objects list
    	node != LIST_EOF;
    	node = list_next(state->objects, node)) {

		Object obj = list_node_value(state->objects, node);	// recover object from list

		if(obj->rect.y <= y_from && obj->rect.y >= y_to) {	// if the object's y coordinate is between
			list_insert_next(list, LIST_BOF, obj);			// y_from and y_to, insert object in the list
		}
	}

	return list;	//list with every object between y_from and y_to y coordinate
}


// Reinitializes state when player wants to play another round

void restart_game(State state) {
	if (state->info.missile != NULL)// if there's a missile,
		free(state->info.missile);	// free its memory

	// Reinitialize state information
	state->info.playing = true;
	state->info.paused = false;
	state->info.score = 0;
	state->info.missile = NULL;
	state->speed_factor = 1;

	state->info.jet->rect.x = (SCREEN_WIDTH - 35)/2;
	state->info.jet->rect.y = 0;

	// Destroy and create new list for objects
	list_destroy(state->objects);
	state->objects = list_create(free);
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

// Checks if missile comes in contact with any objects from the list included in state
// If it collides with a terain object, destroy the missile
// If it collides with any other object, destroy the missile, add 10 point to the score
// and remove the object from the list

void missile_collision(State state, Object missile) {
	if (missile != NULL) {
		List list = state->objects;		// recover list of objects
		Rectangle missile_rect = state->info.missile->rect;	// recover missile dimensions

		ListNode node_before = LIST_BOF;	// used to remove object from list
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
					list_remove_next(list, node_before);	// use previous node to remove object
					free(state->info.missile);	// destroy missile
					state->info.missile = NULL;
				}
				return;
			}

			node_before = node;
		}
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
		jet->rect.y -= 6 * speed; // 6 pixels upwards multiplied by game's speed
	else if (keys->down)
		jet->rect.y -= 2 * speed; // 2 pixels upwards multiplied by game's speed
	else
		jet->rect.y -= 3 * speed; // 3 pixels upwards multiplied by game's speed

	if (keys->left && keys->right)	// If both left and right arrows are
		return;						// pressed don't move left or right
	else if (keys->left)
		jet->rect.x -= 3 * speed;	// 3 pixels left multiplied by game's speed
	else if (keys->right)
		jet->rect.x += 3 * speed;	// 3 pixels right multiplied by game's speed
}

// Checks if jet comes in contact with any objects from the list
// If it does it returns "true", else it returns "false"

bool jet_collision(List list, Rectangle jet_rect) {
	bool collision = false;
	
	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Object obj = list_node_value(list, node); // recover object from list
		collision = CheckCollisionRecs(		// check if jet and object collide
			jet_rect, obj->rect
		);

		if (collision) return true;	// if they collide, return "true"
	}
	
	return false;	// if none of the objects of the list
					// collide with the jet, return "false"
}


// Move enemies depending on the the direction
// they are facing and on the game's speed

void enemy_movement(Object enemy, float speed) {
	int pixels;
	if (enemy->type == HELICOPTER)	// enemies move differently
		pixels = 4;	// helictopers move 4 pixels
	else
		pixels = 3; // warships move 3 pixels

	if (enemy->forward)	// depending on their direction
						// they move left or right
		enemy->rect.x += pixels;	// move "pixels" right multiplied by game's speed
	else
		enemy->rect.x -= pixels;	// move "pixels" left multiplied by game's speed
}

// Checks if an enemy comes in contact with another object (terain) 
// If it does change its direction, else don't

void enemy_collision(Object enemy, Object object) {
	bool collision = false;
	
	collision = CheckCollisionRecs(	// does the enemy collide with the object?
		enemy->rect, object->rect
	);
				
	if (collision)	// if yes, change enemy's direction
		enemy->forward = !enemy->forward;
}


// Finds the last bridge of the current state and returns it

Object find_last_bridge(List list) {
	Object bridge;
	ListNode node = list_first(list);	// recover first node of list
	for (int i = 0 ; i < list_size(list) - 3 ; i++) {	// iterate  list till we
														// get to the last bridge
		bridge = list_node_value(list, node);	//recover object from list
    	node = list_next(list, node);	//move to next node of the list
	}

	return bridge;
}


// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {

	//////// GAME OVER & PAUSED MODES ////////

	// If the game is over don't go deeper in the function (return)

	if (!state->info.playing) {
		if (keys->enter)	// If enter is pressed restart game
			restart_game(state);
		
		return;
	}

	// Pause(or unpause) game if the p key is pressed

	if (keys->p)
		state->info.paused = !state->info.paused;
	
	// Paused mode
	// If the game is paused don't go deeper in the function to
	// prevent state from updating till the player unpauses the game

	// Debug mode (can only be accessed if game is paused)
	// The game will only move while the n key is pressed (1 frame)

	if (state->info.paused && !keys->n)
		return;


	//////// MISSILE ////////

	// The following functions handle everything that has to do with the missile
	// More information about them above the state_update function (line 154 - 231)

	missile_fire(state_info(state), keys->space);

	missile_movement(state->info.missile, state->speed_factor);

	missile_collision(state, state->info.missile);

	missile_destroy(state_info(state));


	//////// JET ////////

	// The functions jet_movement and jet_collision handle everything that has to do with the jet
	// More information about them above the state_update function (line 234 - 273)

	jet_movement(state->info.jet, state->speed_factor, keys);

	// List for jet_collision function
	List list = state_objects(
		state,
		state->info.jet->rect.y + state->info.jet->rect.height,
		state->info.jet->rect.y - 4 * SPACING
	);

	if (jet_collision(list, state->info.jet->rect)) {	// If the jet collides with 
		state->info.playing = false;					// an object, stop the game
		list_destroy(list);
		return;
	}

	list_destroy(list);

	
	//////// ENEMIES ////////

	// The functions enemy_movement and enemy_collision handle everything that has to do with the enemies
	// More information about them above the state_update function (line 276 - 305)

	// Create list of objects near the jet
	List list1 = state_objects(
		state,
		state->info.jet->rect.y + SCREEN_HEIGHT/2,
		state->info.jet->rect.y - SCREEN_HEIGHT
	);
	
	for	(ListNode node = list_first(list1);	// iterate list1
    	node != LIST_EOF;
    	node = list_next(list1, node)) {

		Object enemy = list_node_value(state->objects, node);	// recover object from list1
		if (enemy->type == HELICOPTER || enemy->type == WARSHIP) {
			
			enemy_movement(enemy, state->speed_factor);

			List list2 = state_objects(		// create list of objects near the enemy
				state,
				enemy->rect.y - SPACING,
				enemy->rect.y - 3*SPACING
			);

			for	(ListNode node = list_first(list2);	// iterate list2
				node != LIST_EOF;
				node = list_next(list2, node)) {
				
				Object object = list_node_value(list2, node);	// recover object from list2
				if (object->type == TERAIN)		// enemies collide only with terain objects
					enemy_collision(enemy, object);
			}

			list_destroy(list2);
		}		
	}

	list_destroy(list1);

	//////// ADD OBJECTS & INCREASE SPEED ////////

	// If jet is closer than one screen to the last bridge create more objects
	// and also increase the game's speed by 30%

	Object last_bridge = find_last_bridge(state->objects);
	if (abs(last_bridge->rect.y - state->info.jet->rect.y) < SCREEN_HEIGHT) {
		add_objects(state, last_bridge->rect.y);
		state->speed_factor += 0.3 * state->speed_factor;
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy (State state) {
	if (state->info.missile != NULL)// if there's a missile,
		free(state->info.missile);	// free its memory

	free(state->info.jet);	// free jet's memory

	list_destroy(state->objects);	// free objects' memory

	free(state);	// finally free the state itself						
}