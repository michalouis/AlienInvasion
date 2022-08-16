#include "game_screen.h"

// enemy types
typedef enum {
	LONGHORN, CRAB, MOTHERSHIP, MOTHERSHIP_DEFENDER
} EnemyType;

// enemy struct
typedef struct enemy {
	EnemyType type;	
	Rectangle rect;	
	float bullet_cooldown;
	bool right;					// true: enemy moves to the right
}* Enemy;

// Update everything enemy related

void enemies_update(Game game);

// CompareFunc for set used for enemies

int enemies_comparefunc(Pointer, Pointer);

// Adds enemies to the game

void add_enemies(Game, float);

// Returns list of enemies from y_from to y_to coordinates

List state_enemies(Set, float y_from, float y_to);