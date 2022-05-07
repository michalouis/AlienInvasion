#include "start_game.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <stdio.h>

void missile_movement(Object missile, float speed) {
	printf("MISSILE_MOVEMENT IN\n");

	// if (set_size(missiles) != 0) {
	
	// 	printf("SIZE: %d\n", set_size(missiles));
	// 	for(SetNode node = set_first(missiles);
	// 		node != SET_EOF;
	// 		printf("MOVE!!!!!!!!\n"), node = set_next(missiles, node)) {

				// printf("MOVE IN!!!\n");
				// Object missile = set_node_value(missiles, node);
				missile->rect.y -= 10 * speed;	// 10 pixels upwards multiplied by game's speed
				// printf("MOVE OUT!!!\n");
	// 		}
	// }

	printf("MISSILE_MOVEMENT OUT\n");
}

bool missile_collision(GameState gamestate, Object missile, List list) {
	printf("MISSILE_COLLISION IN\n");

	// if (set_size(gamestate->missiles) != 0) {

		// for(SetNode node = set_first(gamestate->missiles);
		// 	node != SET_EOF;
		// 	node = set_next(gamestate->missiles, node)) {

			// Object missile = set_node_value(gamestate->missiles, node);
			Rectangle missile_rect = missile->rect;	// recover missile dimensions

			// List list = state_objects(	//create list
			// 	gamestate,
			// 	missile->rect.y + missile->rect.height,
			// 	missile->rect.y - 4 * SPACING
			// );

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
						// set_remove(gamestate->missiles, missile);	// destroy missile
                        return true;
						// missile = NULL;
					} else {
						gamestate->score += 10;	// increase score
						set_remove(gamestate->objects, enemy);		// remove object
						// set_remove(gamestate->missiles, missile);	// destroy missile
                        return true;
						// missile = NULL;
					}
					// list_destroy(list);	//free list memory
					// return;
				}
			}

			list_destroy(list);	//free list memory
	
    return false;
    printf("MISSILE_COLLISION OUT\n");
}

bool missile_distance(GameState gamestate, Object missile){
    printf("MISSILE_DESTROY IN\n");

	// Set missiles = gamestate->missiles;

	// List remove_missiles = list_create(NULL);
	// if (set_size(gamestate->missiles) != 0) {
	// 	for(SetNode node = set_first(gamestate->missiles);
	// 		node != SET_EOF;
	// 		node = set_next(gamestate->missiles, node)) {

	// 		Object missile = set_node_value(gamestate->missiles, node);

			float jet_y = gamestate->jet->rect.y;	// recover jet's y coordinate
			float missile_y = missile->rect.y;	// recover missile's y coordinate
				
			if (abs(missile_y - jet_y) > SCREEN_HEIGHT) {
				// list_insert_next(remove_missiles, list_first(remove_missiles), missile);
                return true;
		// 	}
		// }

		// if (list_size(remove_missiles) != 0) {
		// 	for(ListNode node = list_first(remove_missiles);
		// 		node != LIST_EOF;
		// 		node = list_next(remove_missiles, node)) {
		// 			set_remove(gamestate->missiles, list_node_value(remove_missiles, node));
		// 		}
		// }
		// list_destroy(remove_missiles);	
	}

	printf("MISSILE_DESTROY OUT\n");
    return false;
}

void missiles_destroy(List list, GameState gamestate) {
    for	(ListNode node = list_first(list);	// iterate list
		node != LIST_EOF;
		node = list_next(list, node)) {
            set_remove(gamestate->missiles, list_node_value(list, node));
        }
}

void missiles_update(GameState gamestate) {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA\n");

    Set missiles = gamestate->missiles;
    // Set enemies = gamestate->objects;
    float speed = gamestate->speed_factor;
    List remove_missiles = list_create(NULL);

    for(SetNode node = set_first(missiles);
		node != SET_EOF;
		node = set_next(missiles, node)) {

            Object missile = set_node_value(missiles, node);

            missile_movement(missile, speed);

            List list = state_objects(	//create list
				gamestate,
				missile->rect.y + missile->rect.height,
				missile->rect.y - 4 * SPACING
			);
            
            if (missile_collision(gamestate, missile, list)) {
                list_insert_next(remove_missiles, list_last(remove_missiles), missile);
                continue;
            }

            if (missile_distance(gamestate, missile)) {
                list_insert_next(remove_missiles, list_last(remove_missiles), missile);
            }
        }

    missiles_destroy(remove_missiles, gamestate);
    list_destroy(remove_missiles);

    printf("ZZZZZZZZZZZZZZZZZZZZZZZZ\n");
}

Object create_objects(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

void missile_create(GameState gamestate, bool key_pressed) {
    printf("MISSILE_FIRE IN\n");

	if ((set_size(gamestate->missiles) < 3) && key_pressed) {
		Rectangle jet_rect = gamestate->jet->rect;	// recover jet's coordinates

		Object missile = create_objects(			// create missile
			MISSLE,
			jet_rect.x + (jet_rect.width)/2,	// missile's starting position
			jet_rect.y,							// is the center of the jet's position
			5,
			15
		);

		set_insert(gamestate->missiles, missile);
	}
	printf("MISSILE_FIRE OUT\n");
}