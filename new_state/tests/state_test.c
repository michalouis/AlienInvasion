//////////////////////////////////////////////////////////////////
//
// Tests for state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Simple library used for unit testing

#include "state.h"
#include "stdio.h"
#include "state_update_title_scr.h"
#include "raylib.h"
// #include "ADTSet.h"
// #include "set_utils.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	TEST_CHECK(state->name == TITLE_SCREEN);

	// TEST_ASSERT(state->info != NULL);

	TitleScreen title_info = state_info(state);
	TEST_ASSERT(title_info != NULL);

	TEST_CHECK(title_info->button_selected == 0);

	TEST_CHECK(title_info->button1->posX == 0);
	TEST_CHECK(title_info->button1->posY == 0);
	TEST_CHECK(title_info->button1->pressed == false);

	TEST_CHECK(title_info->button2->posX == 0);
	TEST_CHECK(title_info->button2->posY == 200);
	TEST_CHECK(title_info->button2->pressed == false);

	TEST_CHECK(title_info->game_title_text->posX == 250);
	TEST_CHECK(title_info->game_title_text->posY == 350);
	TEST_CHECK(title_info->game_title_text->fontSize == 50);


	//////// Tests for state_objects function ////////

	
		
	// state_destroy(state);
}



// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	// { "test_state_update", test_state_update },
	// { "test_set_utils", test_set_utils },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};