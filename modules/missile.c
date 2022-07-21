#include "missile.h"
#include "ADTList.h"
#include "ADTSet.h"
#include "game_screen.h"
#include "enemies.h"
#include "jet.h"

#include <stdlib.h>

// Update missile's position

static void missile_movement(Missile missile, float speed) {
	if (missile->type == P_MISSILE) {
		missile->rect.y -= 8 * speed;	// 10 pixels upwards multiplied by game's speed
	} else if (missile->type == LH_MISSILE) {
		int pixels = missile->upwards ? -5 : 3;
		missile->rect.y += pixels * speed;	// 10 pixels upwards multiplied by game's speed
	} else if (missile->type == C_MISSILE || missile->type == M_MISSILE) {
		int pixels = 2;	// 2 pixels diagonally multiplied by game's speed

		if (missile->right && missile->upwards) {
			missile->rect.x += pixels * speed;
			missile->rect.y -= pixels * speed;
		} else if (!missile->right && missile->upwards) {
			missile->rect.x -= pixels * speed;
			missile->rect.y -= pixels * speed;
		} else if (missile->right && !missile->upwards) {
			missile->rect.x += pixels * speed;
			missile->rect.y += pixels * speed;
		} else {
			missile->rect.x -= pixels * speed;
			missile->rect.y += pixels * speed;
		}
	}
}

// Check if missile comes in contact with enemy/player

static bool missile_collision(Game game, Missile missile, GameAssets assets) {
	bool missile_collided = false;
	Rectangle missile_rect = missile->rect;	// recover missile dimensions
	
	if (missile->type == P_MISSILE) {

		List list = state_enemies(	//create list of enemies missile can collide with
			game->enemies,
			missile->rect.y + missile->rect.height,
			missile->rect.y - 5 * SPACING
		);
		
		for	(ListNode node = list_first(list);	// iterate list
			node != LIST_EOF;
			node = list_next(list, node)) {
				
			Enemy enemy = list_node_value(list, node);	// recover enemy
			Rectangle enemy_rect = enemy->rect;			// recover enemy dimensions
			
			bool collision = CheckCollisionRecs(	// does the missile collide with this enemy?
				missile_rect, enemy_rect
			);						

			if (collision) {
				game->jet->missiles++;

				if (game->jet->missiles > 5)	// just in case
					game->jet->missiles = 5;
					
				if (enemy->type != MOTHERSHIP) {	// if they collide, go in
					game->score += 10;						// increase score
					set_remove(game->enemies, enemy);		// remove enemy
					set_remove(game->missiles, missile);	// remove missile
					missile_collided = true;
					PlaySound(assets->sound_hit_enemy);			// play hit_enemy sound
					break;
				} else if (enemy->type == MOTHERSHIP){		// if missile collides with mothership
					set_remove(game->missiles, missile);	// just remove missile
					missile_collided = true;
					break;
				}
			}
		}
		
		list_destroy(list);	//free list memory
	} else {
		Jet jet = game->jet;	// recover enemy
		Rectangle jet_rect = jet->rect;			// recover enemy dimensions

		bool collision = CheckCollisionRecs(	// does the missile collide with this player?
			missile_rect, jet_rect
		);

		if(collision) {
			set_remove(game->missiles, missile);	// remove missile
			missile_collided = true;
			if (!game->jet->shield) {
				if (!game->jet->hit)
					PlaySound(assets->sound_hit_player);	// play hit_player sound

				game->jet->hit = true;	// update player hit state to true
			}
		}
	}

	return missile_collided;
}

// If missile goes off screen, remove missile

static bool missile_distance(Game game, Missile missile){
	// Y AXIS //
	if (missile->rect.y < game->camera_y - missile->rect.height) {
		if(missile->type == P_MISSILE)	// if it is a player missile
			game->jet->missiles++;		// increase player's available missiles

		if (game->jet->missiles > 5)	// just in case
			game->jet->missiles = 5;

		set_remove(game->missiles, missile);
		return true;
	} else if (missile->rect.y > game->camera_y + SCREEN_HEIGHT) {
		set_remove(game->missiles, missile);
		return true;
	// X AXIS //
	} else if(missile->rect.x < 0 || missile->rect.x > SCREEN_W_G) {
		set_remove(game->missiles, missile);
		return true;
	}

	return false;
}

void missiles_update(Game game, GameAssets assets) {	// assets needed for sounds
    Set missiles = game->missiles;
    float speed = game->speed_factor;

	for(SetNode node = set_first(missiles);
		node != SET_EOF;
		node = set_next(missiles, node)) {

		Missile missile = set_node_value(missiles, node);

        if (missile_collision(game, missile, assets)) { // if true, set arrangment changed, we have to iterate the list from the start
			if (set_size(missiles) != 0) {	
				node = set_first(missiles);
			} else {
				break;
			}
		}

		if (missile_distance(game, missile)) { // if true, set arrangment changed, we have to iterate the list from the start
			if (set_size(missiles) != 0) {	
				node = set_first(missiles);
			} else {
				break;
			}
		}
	}

	// separate iteration, every missile's position is only updated once
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

static void p_missile_create(Game game, Rectangle obj_rect, MissileType missile_type) {
	Missile missile = malloc(sizeof(*missile));

	missile->type = missile_type;
	missile->rect = (Rectangle) {
		obj_rect.x + (obj_rect.width)/2,	// missile's starting position
		obj_rect.y,							// is the center of the jet's position
		4,
		20
	};
	missile->upwards = true;

	game->jet->missiles--;
	set_insert(game->missiles, missile);
}

static void lh_missile_create(Game game, Rectangle obj_rect, MissileType missile_type) {
	Missile missile = malloc(sizeof(*missile));

	missile->type = missile_type;
	if (game->jet->rect.y < obj_rect.y) {
		missile->rect = (Rectangle) {
			obj_rect.x + (obj_rect.width)/2,	// missile's starting position
			obj_rect.y,							// is the center of the enemy's position
			10,
			10
		};

		missile->upwards = true;
	} else {
		missile->rect = (Rectangle) {
			obj_rect.x + (obj_rect.width)/2,	// missile's starting position
			obj_rect.y + obj_rect.height,		// is the center of the enemy's position
			10,
			10
		};

		missile->upwards = false;
	}

	set_insert(game->missiles, missile);
}

static void c_missile_create(Game game, Rectangle obj_rect, MissileType missile_type) {
	Missile missile1 = malloc(sizeof(*missile1));
	Missile missile2 = malloc(sizeof(*missile2));

	missile1->type = missile_type;
	missile2->type = missile_type;

	Rectangle missile_rect;
	if (game->jet->rect.y < obj_rect.y) {
		missile_rect = (Rectangle) {
			obj_rect.x + (obj_rect.width)/2,	// missile's starting position
			obj_rect.y,							// is the center of the enemy's position
			10,
			10
		};

		missile1->upwards = true;
		missile2->upwards = true;
	} else {
		missile_rect = (Rectangle) {
			obj_rect.x + (obj_rect.width)/2,	// missile's starting position
			obj_rect.y + obj_rect.height,		// is the center of the enemy's position
			10,
			10
		};

		missile1->upwards = false;
		missile2->upwards = false;
	}

	// same starting position, but one missile goes
	// to the left and the other to the right
	missile1->rect = missile_rect;
	missile1->right = false;

	missile2->rect = missile_rect;
	missile2->right = true;

	set_insert(game->missiles, missile1);
	set_insert(game->missiles, missile2);
}

static void m_missile_create(Game game, Rectangle obj_rect, MissileType missile_type) {
	for (int i = 0; i < 3; i++) {
		Missile missile1 = malloc(sizeof(*missile1));
		Missile missile2 = malloc(sizeof(*missile2));

		missile1->type = missile_type;
		missile2->type = missile_type;

		Rectangle missile_rect;
		// Is mothership on the left or right size of the screen?
		if (obj_rect.x != 0) {
			missile_rect = (Rectangle) {
				obj_rect.x + 25,							// missile's starting position
				obj_rect.y + (i+1) * obj_rect.height / 4,	// depends on the "for" loop
				10,
				10
			};

			missile1->right = false;
			missile2->right = false;
		} else {
			missile_rect = (Rectangle) {
				obj_rect.width - 25,						// missile's starting position
				obj_rect.y + (i+1) * obj_rect.height / 4,	// depends on the "for" loop
				10,
				10
			};

			missile1->right = true;
			missile2->right = true;
		}

		missile1->rect = missile_rect;
		missile1->upwards = true;

		missile2->rect = missile_rect;
		missile2->upwards = false;

		set_insert(game->missiles, missile1);
		set_insert(game->missiles, missile2);
	}
}

void missile_create(Game game, Rectangle obj_rect, MissileType missile_type) {
	switch (missile_type) {
		case P_MISSILE:
			p_missile_create(game, obj_rect, missile_type);
			break;
		case LH_MISSILE:
			lh_missile_create(game, obj_rect, missile_type);
			break;
		case C_MISSILE:
			c_missile_create(game, obj_rect, missile_type);
			break;
		case M_MISSILE:
			m_missile_create(game, obj_rect, missile_type);
			break;
	}
}