#include "missile.h"
#include "start_game.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <stdio.h>

void missile_movement(Missile missile, float speed) {
	if (missile->type == P_MISSILE) {
		missile->rect.y -= 10 * speed;	// 10 pixels upwards multiplied by game's speed
	} else if (missile->type == E_MISSILE) {
		missile->rect.y += 5 * speed;	// 10 pixels upwards multiplied by game's speed
	}
}

bool missile_collision(GameState gamestate, Missile missile, List list) {
	bool missile_collided = false;
	Rectangle missile_rect = missile->rect;	// recover missile dimensions
	
	if (missile->type == P_MISSILE) {
		
		for	(ListNode node = list_first(list);	// iterate list
			node != LIST_EOF;
			node = list_next(list, node)) {
				
			Object enemy = list_node_value(list, node);	// recover object
			Rectangle enemy_rect = enemy->rect;			// recover object dimensions
			
			bool collision = CheckCollisionRecs(	// does the missile collide with this object?
				missile_rect, enemy_rect
			);						
				
			if (collision) {	// if they collide, go in
				gamestate->score += 10;	// increase score
				set_remove(gamestate->objects, enemy);		// remove object
				set_remove(gamestate->missiles, missile);
				missile_collided = true;
				break;
			}
		}
		
		list_destroy(list);	//free list memory
	} else {
		Object jet = gamestate->jet;	// recover object
		Rectangle jet_rect = jet->rect;			// recover object dimensions

		bool collision = CheckCollisionRecs(	// does the missile collide with this object?
			missile_rect, jet_rect
		);

		if(collision) {
			set_remove(gamestate->missiles, missile);	// remove object
			missile_collided = true;
			if (!gamestate->hit) {
				gamestate->hearts--;						// an object, stop the game
				gamestate->hit = true;
				gamestate->invis_t_start = time(NULL);
			}
		}
	}

	return missile_collided;
}

bool missile_distance(GameState gamestate, Missile missile){
	printf("GO IN\n");
	if (missile->rect.y < gamestate->camera_y - missile->rect.height) {
		set_remove(gamestate->missiles, missile);
		printf("REMOVED\n");
		printf("GO OUT\n");
		return true;
	} else if (missile->rect.y > gamestate->camera_y + SCREEN_HEIGHT) {
		set_remove(gamestate->missiles, missile);
		printf("REMOVED\n");
		printf("GO OUT\n");
		return true;
	}

	printf("GO OUT\n");
	return false;
}

// void missiles_destroy(Missile missile, GameState gamestate) {
//     // for(ListNode node = list_first(list);	// iterate list
// 	// 	node != LIST_EOF;
// 	// 	node = list_next(list, node)) {
//     //         list_remove(gamestate->missiles, list_node_value(list, node));
//     // }

// 	ListNode node_before = LIST_BOF;	// used to remove object from list
// 	for	(ListNode node = list_first(gamestate->missiles);	// iterate list
// 		node != LIST_EOF;
// 		node = list_next(gamestate->missiles, node)) {

// 		if (list_node_value(gamestate->missiles, node) == missile)
// 			list_remove_next(gamestate->missiles, node_before);


// 		node_before = node;
// 	}

// 	// int size = list_size(list);
// 	// for(int i = 0 ; i < size ; i++) {
// 	// 	list_remove_next(list, LIST_BOF);
// 	// }
// }

void missiles_update(GameState gamestate) {
    Set missiles = gamestate->missiles;
    float speed = gamestate->speed_factor;
    // List remove_missiles = list_create(free);


	// SetNode prev_node = SET_BOF;
	for(SetNode node = set_first(missiles);
		node != SET_EOF;
		node = set_next(missiles, node)) {

		printf("AGAIN\n");
		Missile missile = set_node_value(missiles, node);

		List list = state_objects(	//create list
			gamestate,
			missile->rect.y + missile->rect.height,
			missile->rect.y - 4 * SPACING
		);
            
        if (missile_collision(gamestate, missile, list)) {
			printf("NEXT\n");
			if (set_size(missiles) != 0) {
				node = set_first(missiles);
			} else {
				break;
			}
		}

		if (missile_distance(gamestate, missile)) {
			printf("NEXT\n");
			if (set_size(missiles) != 0) {
				node = set_first(missiles);
			} else {
				break;
			}
		}
	}

	printf("SIZE: ");
	printf("%d\n", set_size(missiles));
	
    for(SetNode node = set_first(missiles);
		node != SET_EOF;
		printf("MPIKA4\n"), node = set_next(missiles, node)) {

		printf("MPIKA1\n");
        Missile missile = set_node_value(missiles, node);

		printf("MPIKA2\n");
        missile_movement(missile, speed);

		printf("MPIKA3\n");
	}

	printf("----------\n");
}

void missile_create(GameState gamestate, Object obj, MissileType missile_type) {
	// if ((set_size(gamestate->missiles) < 3)) {
	Missile missile = malloc(sizeof(*missile));

	missile->type = missile_type;

	Rectangle obj_rect = obj->rect;			// recover jet's coordinates
	Rectangle missile_rect;
	if(missile_type == P_MISSILE) {

		missile_rect = (Rectangle) {
			obj_rect.x + (obj_rect.width)/2,	// missile's starting position
			obj_rect.y,							// is the center of the jet's position
			5,
			15
		};
	} else {
		missile_rect = (Rectangle) {
			obj_rect.x + (obj_rect.width)/2,	// missile's starting position
			obj_rect.y + obj_rect.height,		// is the center of the jet's position
			5,
			15
		};
	}

	missile->rect = missile_rect;

	set_insert(gamestate->missiles, missile);
}