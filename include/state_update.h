#include "state.h"
#include "ADTSet.h"
#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>

// Reinitializes state when player wants to play another round

void restart_game(State);

// Creates a missile if there isn't one already

void missile_fire(StateInfo, bool);

// Moves missile depending on the game's speed

void missile_movement(Object, float);

// Checks if missile comes in contact with any objects from the list
// If it collides with a terain object, destroy the missile
// If it collides with any other object, destroy the missile, add 10 point to the score
// and remove the object from the set

void missile_collision(State, Object, Set);

// Checks if the missile is too far away from the jet 
// and destroys it if it is

void missile_destroy(StateInfo);

// Moves jet depending on the pressed keys
// and the game's speed

void jet_movement(Object, float, KeyState);

// Checks if jet comes in contact with any objects from the list
// If it does it returns "true", else it returns "false"

bool jet_collision(State, Rectangle);

// Checks if an enemy is about to collide with a terain object 
// If it does change its direction, else don't

void enemy_collision(Object, Set);

// Move enemies depending on the the direction
// they are facing and on the game's speed

void enemy_movement(Object, float);

// Finds the last bridge of the current state and returns it

Object find_last_bridge(Set set);