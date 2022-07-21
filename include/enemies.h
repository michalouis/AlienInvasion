#include "game_screen.h"

typedef enum {
	LONGHORN, CRAB, MOTHERSHIP, MOTHERSHIP_DEFENDER
} EnemyType;

// Πληροφορίες για κάθε αντικείμενο
typedef struct enemy {
	EnemyType type;				// Τύπος (Εδαφος / Ελικόπτερο / Πλοίο / Αεροσκάφος / Πύραυλος / Γέφυρα)
	Rectangle rect;					// Θέση και μέγεθος του αντικειμένου. Το Rectangle ορίζεται στο include/raylib.h, line 213
	float bullet_cooldown;
	bool right;					// true: το αντικείμενο κινείται προς τα δεξιά
}* Enemy;

// Update everything enemy related

void enemies_update(Game game);

// CompareFunc for set used for enemies

int enemies_comparefunc(Pointer, Pointer);

// Adds enemies to the game

void add_enemies(Game, float);

// Returns list of enemies from y_from to y_to coordinates

List state_enemies(Set, float y_from, float y_to);