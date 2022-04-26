#include <stdlib.h>

#include "ADTList.h"
#include "ADTSet.h"
#include "set_utils.h"
#include "state.h"
#include "raylib.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Set objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
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
//   συντεταγμένες x είναι ανάμεσα στο 0 και το SCREEN_W_R.
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
			SCREEN_W_R,						// Πλάτος ολόκληρη η οθόνη
			20								// Υψος
		);

		// Δημιουργία εδάφους
		Object terain_left = create_object(
			TERAIN,
			0,								// Αριστερό έδαφος, x = 0
			bridge->rect.y,					// y ίδιο με την γέφυρα
			rand() % (SCREEN_W_R/3),		// Πλάτος επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);
		int width = rand() % (SCREEN_W_R/2);
		Object terain_right = create_object(
			TERAIN,
			SCREEN_W_R - width,			// Δεξί έδαφος, x = <οθόνη> - <πλάτος εδάφους>
			bridge->rect.y,					// y ίδιο με τη γέφυρα
			width,							// Πλάτος, επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);

		set_insert(state->objects, terain_left);
		set_insert(state->objects, terain_right);
		set_insert(state->objects, bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_W_R - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_W_R - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			set_insert(state->objects, enemy);
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
		
		// Special case: bridge asset won't be placed above terain
		if (obj_a->type == TERAIN && obj_b->type == BRIDGE) {
			return 1;
		}
		if(obj_a->type == BRIDGE && obj_b->type == TERAIN) {
			return -1;
		}

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

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->info.score = 0;					// Αρχικό σκορ 0
	state->info.hearts = 3;
	state->info.missile = NULL;				// Αρχικά δεν υπάρχει πύραυλος
	state->speed_factor = 1;				// Κανονική ταχύτητα

	state->info.camera_x = SCREEN_W_R / 2;
	state->info.camera_y = -(SCREEN_HEIGHT / 2);

	// Δημιουργία του αεροσκάφους, κεντραρισμένο οριζόντια και με y = 0
	state->info.jet = create_object(JET, (SCREEN_W_R - 35)/2,  0, 35, 40);

	// Δημιουργούμε τo σετ των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	state->objects = set_create(compare_objects, free);
	add_objects(state, 0);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη x είναι ανάμεσα στο x_from και x_to.

List state_objects(State state, float y_from, float y_to) {
	List list = list_create(NULL);	// create list


	Object obj1 = malloc(sizeof(Object));	// allocate memory
	obj1->rect.y = y_from;	// give y coordinate to obj1 
							// (required for the next functions)

	Object obj2 = malloc(sizeof(Object));	// allocate memory
	obj2->rect.y = y_to;	// give y coordinate to obj2
							// (required for the next functions)

	Set set = state->objects;

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

	state->info.jet->rect.x = (SCREEN_W_R/2 - 35)/2;
	state->info.jet->rect.y = 0;

	state->info.camera_x = SCREEN_W_R / 2;
	state->info.camera_y = -(SCREEN_HEIGHT / 2);

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


// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	Set set = state->objects;

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

	
	state->info.camera_y -= 3;


	//////// MISSILE ////////

	// The following functions handle everything that has to do with the missile
	// More information about them above the state_update function (line 212 - 283)

	missile_fire(state_info(state), keys->space);

	missile_movement(state->info.missile, state->speed_factor);

	missile_collision(state, state->info.missile, set);	

	missile_destroy(state_info(state));


	//////// JET ////////

	// The functions jet_movement and jet_collision handle everything that has to do with the jet
	// More information about them above the state_update function (line 286 - 325)

	jet_movement(state->info.jet, state->speed_factor, keys);

	if (jet_collision(state, state->info.jet->rect)) {	// If the jet collides with 
		state->info.hearts--;							// an object, stop the game
	}

	if (!state->info.hearts) {
		state->info.playing = false;
		return;
	}

	
	/////// ENEMIES ////////

	// The functions enemy_movement and enemy_collision handle everything that has to do with the enemies
	// More information about them above the state_update function (line 328 - 384)

	// Create list of objects near the jet
	List list = state_objects(
		state,
		state->info.jet->rect.y + SCREEN_HEIGHT/2,
		state->info.jet->rect.y - SCREEN_HEIGHT
	); 

	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Object enemy = list_node_value(list, node);	// recover object
		if (enemy->type == HELICOPTER || enemy->type == WARSHIP) {
			
			enemy_collision(enemy, set);
			enemy_movement(enemy, state->speed_factor);	
		}
	}

	list_destroy(list);

	//////// ADD OBJECTS & INCREASE SPEED ////////

	// If jet is closer than one screen to the last bridge create more objects
	// and also increase the game's speed by 30%

	float last_bridge_y = find_last_bridge(set)->rect.y;
	if (abs(last_bridge_y - state->info.jet->rect.y) < SCREEN_HEIGHT) {
		add_objects(state, last_bridge_y);
		state->speed_factor += 0.3 * state->speed_factor;
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy (State state) {
	if (state->info.missile != NULL)// if there's a missile,
		free(state->info.missile);	// free its memory

	free(state->info.jet);	// free jet's memory

	set_destroy(state->objects);	// free objects' memory

	free(state);	// finally free the state itself
}