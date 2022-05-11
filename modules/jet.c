#include "jet.h"
#include "start_game.h"
#include "enemies.h"
#include "ADTList.h"
#include "state.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

Jet jet_create(float x, float y, float width, float height) {
    Jet jet = malloc(sizeof(*jet));

    jet->rect.x = x;
    jet->rect.y = y;
    jet->rect.width = width;
    jet->rect.height = height;;
    jet->hearts = 3;
    jet->hit = false;
    jet->invis_t_start = 0;

    return jet;
}

// Moves jet depending on the pressed keys
// and the game's speed

void jet_movement(Jet jet, float speed, float camera_y, KeyState keys) {
    // Jet jet = gamestate->jet;
    // float camera_x = gamestate->camera_x;
    // float camera_y = gamestate->camera_y;

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

void jet_collision(Set objects, Jet jet) {
    if (jet->hit)
        return;

    bool collision = false;
    Rectangle jet_rect = jet->rect;

	List list = state_enemies(	//create list
		objects,
		jet->rect.y + jet->rect.height,
		jet->rect.y - 4 * SPACING
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
			jet->hit = true;		// if they collide, return "true"
            return;
		}	
	}

	list_destroy(list);		// free list memory
	return;     			// if none of the objects of the list
							// collide with the jet, return "false"
}

void jet_hit(Jet jet) {
    if (jet->hit && jet->invis_t_start == 0) {
        jet->hearts--;
        jet->invis_t_start = time(NULL);
    } 
    
    if (jet->hit && jet->invis_t_start) {
        time_t t_now = time(NULL);

		if(t_now > jet->invis_t_start + 5) {
			jet->invis_t_start = 0;
			jet->hit = false;
		}
    }
}

void jet_update(Jet jet, float camera_y, float speed, KeyState keys, Set objects) {
    jet_movement(jet, speed, camera_y, keys);
	jet_collision(objects, jet);
	jet_hit(jet);
}

bool jet_gameover(Jet jet) {
    if (jet->hearts)
        return false;
    else
        return true;
}

Jet jet_reset(Jet jet, float x, float y) {
    jet->rect.x = x;
    jet->rect.y = y;

    jet->hearts = 3;
    jet->hit = false;
    jet->invis_t_start = 0;

    return jet;
}

void jet_destroy(Jet jet) {
    free(jet);
}

