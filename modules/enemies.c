#include "start_game.h"
#include "missile.h"
#include "set_utils.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

List state_enemies(Set objects, float y_from, float y_to) {
	List list = list_create(NULL);	// create list


	Object obj1 = malloc(sizeof(Object));	// allocate memory
	obj1->rect.y = y_from;	// give y coordinate to obj1 
							// (required for the next functions)

	Object obj2 = malloc(sizeof(Object));	// allocate memory
	obj2->rect.y = y_to;	// give y coordinate to obj2
							// (required for the next functions)

	Set set = objects;

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

// Checks if an enemy is about to collide with a terain object 
// If it does change its direction, else don't

void enemy_collision(Object enemy, Set set) {

	// If enemy is about to collide with a terain object change its direction
	if (enemy->rect.x < 0 || enemy->rect.x + enemy->rect.width > SCREEN_W_G)
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

void enemy_missile(Game game, Object enemy) {
	time_t timel = time(NULL);
	if(timel >= enemy->countdown) {
		MissileType m_type;
		if (enemy->type == WARSHIP)
			m_type = W_MISSILE;
		else
			m_type = H_MISSILE;
		
		missile_create(game, enemy->rect, m_type);
		timel = time(NULL);
		enemy->countdown = timel + GetRandomValue(2,3);	
		// enemy->countdown = timel + 1;	
	}
}

void enemies_update(Game game) {
    Set set = game->objects;

    List list = state_enemies(
		game->objects,
		game->camera_y + SCREEN_HEIGHT,
		game->camera_y - 100
	); 

	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Object enemy = list_node_value(list, node);	// recover object
		if (enemy->type == HELICOPTER || enemy->type == WARSHIP) {
			
			enemy_missile(game, enemy);
			enemy_collision(enemy, set);
			enemy_movement(enemy, game->speed_factor);
		}
	}

	list_destroy(list);
	
}

// CompareFunc comparing objects (used for set)

int enemies_comparefunc(Pointer a, Pointer b) {
	Object obj_a = a;
	Object obj_b = b;

	// comparing coordinates of y axis
	if (obj_a->rect.y < obj_b->rect.y) {
		return 1;
	} else if (obj_a->rect.y > obj_b->rect.y) {
		return -1;
	} else {

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

void add_objects(Game game, float start_y) {
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
		set_insert(game->objects, bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_W_G - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_W_G - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			set_insert(game->objects, enemy);
		}
	}
}