#include "jet.h"
#include "game_screen.h"
#include "enemies.h"
#include "beams.h"
#include "ADTList.h"
#include "state.h"

#include <stdlib.h>

Jet jet_create(float x, float y, float width, float height) {
    Jet jet = malloc(sizeof(*jet));	// Δυναμική δέσμευση μνήμης

	// Jet coordinates & dimensions
    jet->rect.x = x;
    jet->rect.y = y;
    jet->rect.width = width;
    jet->rect.height = height;

	// Jet Status
    jet->hearts = 6;
	jet->bar = 0;
    jet->missiles = 5;

	// Initialize shield
	jet->shield = false;
    jet->shield_time = 5;
	jet->shield_cooldown = 0;

	// Initialize hit state
    jet->hit = false;
    jet->invisibility_time = 0;

	// Initialize effect state
    jet->hit_by_beam = false;
    jet->effect_time = 0;


    return jet;
}

// Moves jet depending on the pressed keys
// and the game's speed

static void jet_movement(Jet jet, float speed, float camera_y, KeyState keys) {

	if (keys->up)
		jet->rect.y -= 8 * speed;	// 6 pixels upwards multiplied by game's speed
	else if (keys->down)
		jet->rect.y += 2 * speed;	// 2 pixels upwards multiplied by game's speed
	else
		jet->rect.y -= 3 * speed;	// 3 pixels upwards multiplied by game's speed

	if (keys->left && keys->right)	// If both left and right arrows are
		return;						// pressed don't move left or right
	else if (keys->left)
		jet->rect.x -= 4 * speed;	// 3 pixels left multiplied by game's speed
	else if (keys->right)
		jet->rect.x += 4 * speed;	// 3 pixels right multiplied by game's speed
    
	// Δεν επιτρέπει στο jet να βγει εκτός οθόνης
    if (jet->rect.y < camera_y + 10)
        jet->rect.y = camera_y + 10;

    if (jet->rect.y > camera_y + SCREEN_HEIGHT - jet->rect.height - 10)
        jet->rect.y = camera_y + SCREEN_HEIGHT - jet->rect.height - 10;

    if (jet->rect.x < 10)
        jet->rect.x = 10;
    
    if (jet->rect.x > SCREEN_W_G - jet->rect.width - 10)
        jet->rect.x = SCREEN_W_G - jet->rect.width - 10;
}

// Ενημερώνει τη κατάσταση της ασπίδας
static void jet_shield_update(Jet jet) {
	// If all of the shield energy is used, dont let player use it immediately
	if(jet->shield_cooldown) {
		jet->shield_cooldown -= GetFrameTime() / 4;

		if (jet->shield_cooldown < 0)
			jet->shield_cooldown = 0;
	}

	// Recharge shield when not in use
	if (!jet->shield) {
		if (jet->shield_time != 5)
			jet->shield_time += GetFrameTime() / 4;

		if (jet->shield_time > 5)
			jet->shield_time = 5;

		return;
	}

	// When shield in use, reduce shield energy
	jet->shield_time -= GetFrameTime();

	// If all of the shield energy is used, disable shield
	if(jet->shield_time < 0) {
		jet->shield_time = 0;
		jet->shield = false;
		jet->shield_cooldown = 2.5;
	}
}

// Checks if jet comes in contact with any objects from the list

static void jet_enemy_collision(Set objects, Jet jet, Sound hit_player) {
    if (jet->hit)
        return;

    bool collision = false;
    Rectangle jet_rect = jet->rect;

	List list = state_enemies(	//create list
		objects,
		jet->rect.y + jet->rect.height,
		jet->rect.y - 5 * SPACING
	);

	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Enemy obj = list_node_value(list, node);	// recover object
		collision = CheckCollisionRecs(		// check if jet and object collide
			jet_rect, obj->rect
		);

		if (collision) {
			list_destroy(list);		//free list memory
			jet->hit = true;		// if they collide, return "true"
			PlaySound(hit_player);
            return;
		}	
	}

	list_destroy(list);		// free list memory
	return;     			// if none of the objects of the list
							// collide with the jet, return "false"
}

// Checks if jet comes in contact with any objects from the list

static void jet_beam_collision(Set beams, Jet jet, Sound hit_beam) {
    if (jet->hit)	// while hit, jet is invisible
        return;
    
    bool collision = false;
    Rectangle jet_rect = jet->rect;

	for	(SetNode node = set_first(beams);	// iterate list
    	node != SET_EOF;
    	node = set_next(beams, node)) {

		Beam beam = set_node_value(beams, node);	// recover object
		collision = CheckCollisionRecs(		// check if jet and object collide
			jet_rect, beam->rect
		);

		if (collision) {
			if (!jet->hit_by_beam)
				PlaySound(hit_beam);

			jet->hit_by_beam = true;		// if they collide, return "true"
            return;
		}	
	}

	return;     			// if none of the beams
							// collide with the jet, return "false"
}

// Update hit state

static void jet_hit(Jet jet) {
	// if jet hit
    if (jet->hit && jet->invisibility_time == 0) {
        jet->hearts--;	// reduce hearts by 1
		jet->bar = 0;
        jet->invisibility_time = 3;	// make jet invisible for 3 seconds
        jet->effect_time = 0;	// if jet hit remove effects
		jet->hit_by_beam = false;
    } 
    
	// Update invisibility time, if jet was hit
    if (jet->hit && jet->invisibility_time) {
        jet->invisibility_time -= GetFrameTime();

		// when invisibility time reaches zero, make jet vulnerable again
		if(jet->invisibility_time < 0) {
			jet->invisibility_time = 0;
			jet->hit = false;
		}
    }
}

// Update hit by beam state

static void jet_beam_hit(Jet jet) {
	// if jet hit by beam, put effect on jet
    if (jet->hit_by_beam && jet->effect_time == 0)
        jet->effect_time = 6;
    
	// Update time till effect wears off
    if (jet->hit_by_beam && jet->effect_time) {
        jet->effect_time -= GetFrameTime();

		// if effect time is up, remove effect from jet
		if(jet->effect_time < 0) {
			jet->effect_time = 0;
			jet->hit_by_beam = false;
		}
    }
}

void jet_update(Jet jet, float camera_y, float speed, KeyState keys, Set objects, Set beams, Sound hit_player, Sound hit_beam) {
    jet_movement(jet, speed, camera_y, keys);
	
	// While shield is enabled, jet is invisible
	if (!jet->shield) {
		jet_enemy_collision(objects, jet, hit_player);
		jet_beam_collision(beams, jet, hit_beam);
	}
	
	jet_hit(jet);
	jet_beam_hit(jet);
	jet_shield_update(jet);
}

bool jet_gameover(Jet jet) {
    if (jet->hearts)
        return false;
    else
        return true;
}

Jet jet_reset(Jet jet, float x, float y) {
	// Reinitialize jet coordinates & dimensions
    jet->rect.x = x;
    jet->rect.y = y;

	// Reinitialize jet status
    jet->hearts = 6;
	jet->bar = 0;
    jet->missiles = 5;
	
	// Reinitialize shield
	jet->shield = false;
	jet->shield_time = 5;
	jet->shield_cooldown = 0;

	// Reinitialize hit state
    jet->hit = false;
    jet->invisibility_time = 0;

	// Reinitialize effect state
	jet->hit_by_beam = false;
    jet->effect_time = 0;


    return jet;
}

void jet_destroy(Jet jet) {
    free(jet);
}

