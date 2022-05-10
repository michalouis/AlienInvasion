#include "jet.h"
#include "start_game.h"
#include "enemies.h"
#include "ADTList.h"

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

