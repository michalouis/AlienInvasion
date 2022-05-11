#include "missile.h"
#include "start_game.h"
#include "ADTList.h"
#include "ADTSet.h"
#include "enemies.h"
#include "jet.h"

#include <stdio.h>

void missile_movement(Missile missile, float speed) {
	if (missile->type == P_MISSILE) {
		missile->rect.y -= 10 * speed;	// 10 pixels upwards multiplied by game's speed
	} else if (missile->type == H_MISSILE) {
		int pixels = missile->upwards ? -5 : 5;
		missile->rect.y += pixels * speed;	// 10 pixels upwards multiplied by game's speed
	} else if (missile->type == W_MISSILE) {
		int pixels = 3;

		if (missile->right && missile->upwards) {
			missile->rect.x += pixels;
			missile->rect.y -= pixels;
		} else if (!missile->right && missile->upwards) {
			missile->rect.x -= pixels;
			missile->rect.y -= pixels;
		} else if (missile->right && !missile->upwards) {
			missile->rect.x += pixels;
			missile->rect.y += pixels;
		} else {
			missile->rect.x -= pixels;
			missile->rect.y += pixels;
		}
	}
}

bool missile_collision(Game game, Missile missile, List list) {
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
				game->score += 10;	// increase score
				set_remove(game->objects, enemy);		// remove object
				set_remove(game->missiles, missile);
				missile_collided = true;
				break;
			}
		}
		
		list_destroy(list);	//free list memory
	} else {
		Jet jet = game->jet;	// recover object
		Rectangle jet_rect = jet->rect;			// recover object dimensions

		bool collision = CheckCollisionRecs(	// does the missile collide with this object?
			missile_rect, jet_rect
		);

		if(collision) {
			set_remove(game->missiles, missile);	// remove object
			missile_collided = true;
			game->jet->hit = true;
		}
	}

	return missile_collided;
}

bool missile_distance(Game game, Missile missile){
	if (missile->rect.y < game->camera_y - missile->rect.height) {
		set_remove(game->missiles, missile);
		return true;
	} else if (missile->rect.y > game->camera_y + SCREEN_HEIGHT) {
		set_remove(game->missiles, missile);
		return true;
	} else if(missile->rect.x < 0 || missile->rect.x > SCREEN_W_G) {
		set_remove(game->missiles, missile);
		return true;
	}

	return false;
}

void missiles_update(Game game) {
    Set missiles = game->missiles;
    float speed = game->speed_factor;
    // List remove_missiles = list_create(free);


	// SetNode prev_node = SET_BOF;
	for(SetNode node = set_first(missiles);
		node != SET_EOF;
		node = set_next(missiles, node)) {

		Missile missile = set_node_value(missiles, node);

		List list = state_enemies(	//create list
			game->objects,
			missile->rect.y + missile->rect.height,
			missile->rect.y - 4 * SPACING
		);
            
        if (missile_collision(game, missile, list)) {
			if (set_size(missiles) != 0) {
				node = set_first(missiles);
			} else {
				break;
			}
		}

		if (missile_distance(game, missile)) {
			if (set_size(missiles) != 0) {
				node = set_first(missiles);
			} else {
				break;
			}
		}
	}

    for(SetNode node = set_first(missiles);
		node != SET_EOF;
		node = set_next(missiles, node)) {

        Missile missile = set_node_value(missiles, node);
		
        missile_movement(missile, speed);		
	}

}

int missile_comparefunc(Pointer a, Pointer b) {
	// comparing memory addresses
	if (a < b)
		return 1;
	else if (a > b)
		return -1;
	else
		return 0;
}

void missile_create(Game game, Rectangle obj_rect, MissileType missile_type) {
	if (missile_type != W_MISSILE) {
		Missile missile = malloc(sizeof(*missile));

		missile->type = missile_type;

		// Rectangle obj_rect = obj->rect;			// recover jet's coordinates
		Rectangle missile_rect;
		if(missile_type == P_MISSILE) {

			missile_rect = (Rectangle) {
				obj_rect.x + (obj_rect.width)/2,	// missile's starting position
				obj_rect.y,							// is the center of the jet's position
				5,
				15
			};

			missile->upwards = true;
		} else if (missile_type == H_MISSILE){
			if (game->jet->rect.y < obj_rect.y) {
				missile_rect = (Rectangle) {
					obj_rect.x + (obj_rect.width)/2,	// missile's starting position
					obj_rect.y,		// is the center of the jet's position
					5,
					15
				};

				missile->upwards = true;
			} else {
				missile_rect = (Rectangle) {
					obj_rect.x + (obj_rect.width)/2,	// missile's starting position
					obj_rect.y + obj_rect.height,		// is the center of the jet's position
					8,
					8
				};

				missile->upwards = false;
			}
		}

		missile->rect = missile_rect;

		set_insert(game->missiles, missile);
	} else {
		Missile missile1 = malloc(sizeof(*missile1));
		Missile missile2 = malloc(sizeof(*missile2));

		missile1->type = missile_type;
		missile2->type = missile_type;

		// Rectangle obj_rect = obj->rect;			// recover jet's coordinates
		Rectangle missile_rect;

		if (game->jet->rect.y < obj_rect.y) {
			missile_rect = (Rectangle) {
				obj_rect.x + (obj_rect.width)/2,	// missile's starting position
				obj_rect.y,		// is the center of the jet's position
				8,
				8
			};

			missile1->upwards = true;
			missile2->upwards = true;
		} else {
			missile_rect = (Rectangle) {
				obj_rect.x + (obj_rect.width)/2,	// missile's starting position
				obj_rect.y + obj_rect.height,		// is the center of the jet's position
				8,
				8
			};

			missile1->upwards = false;
			missile2->upwards = false;
		}

		missile1->rect = missile_rect;
		missile1->right = false;

		missile2->rect = missile_rect;
		missile2->right = true;

		set_insert(game->missiles, missile1);
		set_insert(game->missiles, missile2);
	}
}