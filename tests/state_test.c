//////////////////////////////////////////////////////////////////
//
// Tests for state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Simple library used for unit testing

#include "state.h"
#include "stdio.h"
#include "ADTSet.h"
#include "set_utils.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);
	TEST_ASSERT(info->missile == NULL);

	//////// Tests for state_objects function ////////

	// List returned by state_objects should contain at least one object
	// Distance between y_from and y_to is big enough
	// for the list to contain more than 6 objects 
	List list1 = state_objects(state, 0, -1500);
	TEST_CHECK(list_size(list1) != 0 && list_size(list1) > 6);

	// List returned by state_objects should contain at least one object
	// List doesn't contain more than 6 objects because the
	// distance between y_from and y_to isn't big enough
	List list2 = state_objects(state, 0, -800);
	TEST_CHECK(list_size(list2) != 0 && list_size(list2) <= 6);

	// List contains no objects because the distance
	// between y_from and y_to is way too small
	List list3 = state_objects(state, 0, -100);
	TEST_CHECK(list_size(list3) == 0);

	// Checks if every object stored in the
	// list is between y_from and y_to
	List list4 = state_objects(state, -500, -1500);
	for(ListNode node = list_first(list4);
		node != LIST_EOF;
		node = list_next(list4, node)) {

		Object obj = list_node_value(list4, node);
		TEST_CHECK(obj->rect.y <= -500 && obj->rect.y >= -1500);
	}

	// Free memory
	list_destroy(list1);
	list_destroy(list2);
	list_destroy(list3);
	list_destroy(list4);
		
	state_destroy(state);
}

void test_state_update() {

	State state = state_create();
	StateInfo info = state_info(state);
	TEST_ASSERT(state != NULL && info != NULL);

	// Keys information (by default they're not pressed)
	struct key_state keys = { false, false, false, false, false, false, false, false };

	//////// Tests for jet_movement function ////////

	// No keys pressed, jet moves 3 pixels upwards
	Rectangle old_rect = info->jet->rect;
	state_update(state, &keys);
	Rectangle new_rect = info->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 3 );

	// Up arrow pressed, jet moves 6 pixels upwards
	keys.up = true;
	old_rect = info->jet->rect;
	state_update(state, &keys);
	new_rect = info->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 6 );
	keys.up = false;

	// Down arrow pressed, jet moves 2 pixels upwards
	keys.down = true;
	old_rect = info->jet->rect;
	state_update(state, &keys);
	new_rect = info->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 2 );
	keys.down = false;

	// Right arrow pressed, jet moves 3 pixels to the right
	keys.right = true;
	old_rect = info->jet->rect;
	state_update(state, &keys);
	new_rect = info->jet->rect;

	TEST_CHECK( new_rect.y == old_rect.y - 3 && new_rect.x == old_rect.x + 3 );
	keys.right = false;

	// Left arrow pressed, jet moves 3 pixels to the left
	keys.left = true;
	old_rect = info->jet->rect;
	state_update(state, &keys);
	new_rect = info->jet->rect;

	TEST_CHECK( new_rect.y == old_rect.y - 3 && new_rect.x == old_rect.x - 3 );
	keys.left = false;

	//////// Test for missile_movement function ////////

	// create missile
	keys.space = true;
	state_update(state, &keys);
	keys.space = false;

	TEST_ASSERT(info->missile != NULL);

	// missile should move 10 pixels upwards
	old_rect = info->missile->rect;
	state_update(state, &keys);
	new_rect = info->missile->rect;

	TEST_CHECK(new_rect.x == old_rect.x && new_rect.y == old_rect.y - 10);

	//////// Test for missile_collision function ////////

	// recover an object from list and place missile on
	// the same coordiantes as the object
	List list = state_objects(state, 0, -2000);
	int old_list_size = list_size(list);
	
	Object obj = list_node_value(list, list_first(list));
	info->missile->rect.x = obj->rect.x;
	info->missile->rect.y = obj->rect.y + 10;
	state_update(state, &keys);

	list_destroy(list);

	// compare list size before and after collision
	// also check if missile is destroyed
	list = state_objects(state, 0, -2000);
	int new_list_size = list_size(list);

	TEST_CHECK(new_list_size == old_list_size - 1);
	TEST_CHECK(info->missile == NULL);

	list_destroy(list);

	//////// Test for Game Over ////////

	// collide jet with an object
	list = state_objects(state, 0, -SCREEN_HEIGHT);
	obj = list_node_value(list, list_first(list));
	info->jet->rect.x = obj->rect.x;
	info->jet->rect.y = obj->rect.y;

	state_update(state, &keys);

	// check if game is over
	TEST_CHECK(info->playing == false);

	// restart game
	keys.enter = true;
	state_update(state, &keys);
	keys.enter = false;

	// check if game is playing
	TEST_CHECK(info->playing == true);

	list_destroy(list);

	state_destroy(state); // free memory
}


//////////////////////////////////////////////////////////////////
//
// Tests for set_utils.h module
//
//////////////////////////////////////////////////////////////////


// Allocates memory for an integer, copies value of
// integer to a pointer and returns pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// allocate memory
	*pointer = value;						// copy integer's value to pointer
	return pointer;
}

// Compares 2 integers a, b
int compare_ints(Pointer a, Pointer b) {
    int* ia = a;
    int* ib = b;
    return *ia - *ib;       // negative, zero or positive, depending on the arrangement of a,b
}

void test_set_utils() {
	Set set = set_create(compare_ints, free);

	set_insert(set, create_int(5));
	set_insert(set, create_int(15));
	set_insert(set, create_int(-5));
	set_insert(set, create_int(27));

	// Number 5 is stored in the set
	int number = 5;
	number = *(int*)set_find_eq_or_greater(set, &number);
	TEST_CHECK(number == 5);

	// Remove number 5 (required for next tests)
	set_remove(set, &number);

	// Number 5 isn't in the set, return the next greater value
	number = *(int*)set_find_eq_or_greater(set, create_int(5));
	TEST_CHECK(number == 15);

	// Number 5 isn't in the set, return the next smaller value
	number = *(int*)set_find_eq_or_smaller(set, create_int(5));
	TEST_CHECK(number == -5);

	// Set doesn't contain a number greater than 30, return NULL
	TEST_CHECK(set_find_eq_or_greater(set, create_int(30)) == NULL);

	set_destroy(set);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },
	{ "test_set_utils", test_set_utils },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};