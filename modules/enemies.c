#include "start_game.h"
#include "missile.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

void enemy_missile(GameState gamestate, Object enemy) {
	time_t timel = time(NULL);
	if(timel >= enemy->countdown) {
		missile_create(gamestate, enemy, E_MISSILE);
		timel = time(NULL);
		// enemy->countdown = timel + GetRandomValue(1,2);	
		enemy->countdown = timel + 1;	
	}
}

void enemies_update(GameState gamestate) {
    Set set = gamestate->objects;

    List list = state_objects(
		gamestate,
		gamestate->camera_y + SCREEN_HEIGHT,
		gamestate->camera_y - 100
	); 

	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Object enemy = list_node_value(list, node);	// recover object
		if (enemy->type == HELICOPTER || enemy->type == WARSHIP) {
			
			enemy_missile(gamestate, enemy);
			enemy_collision(enemy, set);
			enemy_movement(enemy, gamestate->speed_factor);
		}
	}

	list_destroy(list);

	
}