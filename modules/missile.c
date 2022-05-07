#include "start_game.h"
#include "ADTList.h"
#include "ADTSet.h"
#include "missile.h"

#include <stdio.h>

void missile_movement(Object missile, float speed) {
	missile->rect.y -= 10 * speed;	// 10 pixels upwards multiplied by game's speed
}

bool missile_collision(GameState gamestate, Object missile, List list) {
	bool missile_collided = false;
	Rectangle missile_rect = missile->rect;	// recover missile dimensions

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
					missile_collided = true;
					break;
				} else {
					gamestate->score += 10;	// increase score
					set_remove(gamestate->objects, enemy);		// remove object
					missile_collided = true;
					break;
				}
			}
	}

	list_destroy(list);	//free list memory
	
    return missile_collided;
}

bool missile_distance(GameState gamestate, Object missile){
	float camera_y = gamestate->camera_y;	// recover jet's y coordinate
	float missile_y = missile->rect.y;	// recover missile's y coordinate
	int missile_height = missile->rect.height;
				
	if (missile_y < camera_y - missile_height)
		return true;

    return false;
}

void missiles_destroy(List list, GameState gamestate) {
    for(ListNode node = list_first(list);	// iterate list
		node != LIST_EOF;
		node = list_next(list, node)) {
            set_remove(gamestate->missiles, list_node_value(list, node));
    }
}

void missiles_update(GameState gamestate) {
    Set missiles = gamestate->missiles;
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
}

void missile_create(GameState gamestate, Object obj, bool key_pressed) {
	if ((set_size(gamestate->missiles) < 3) && key_pressed) {

		Missile missile = malloc(sizeof(*missile));

		missile->type = P_MISSILE;

		Rectangle jet_rect = obj->rect;	// recover jet's coordinates
		Rectangle missile_rect = {
			jet_rect.x + (jet_rect.width)/2,	// missile's starting position
			jet_rect.y,							// is the center of the jet's position
			5,
			15
		};
		missile->rect = missile_rect;

		set_insert(gamestate->missiles, missile);
	}
}