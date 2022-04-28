#pragma once

#include "raylib.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <time.h>

#define BRIDGE_NUM 100		// πόσες γέφυρες δημιουργούνται στην πίστα
#define SPACING 200			// απόσταση ανάμεσα στα αντικείμενα της πίστας
#define SCREEN_WIDTH 900	// Πλάτος της οθόνης
#define SCREEN_HEIGHT 800	// Υψος της οθόνης
#define SCREEN_W_R 630

typedef enum {
	TERAIN, HELICOPTER, WARSHIP, JET, MISSLE, BRIDGE
} ObjectType;

// Πληροφορίες για κάθε αντικείμενο
typedef struct object {
	ObjectType type;				// Τύπος (Εδαφος / Ελικόπτερο / Πλοίο / Αεροσκάφος / Πύραυλος / Γέφυρα)
	Rectangle rect;					// Θέση και μέγεθος του αντικειμένου. Το Rectangle ορίζεται στο include/raylib.h, line 213
	bool forward;					// true: το αντικείμενο κινείται προς τα δεξιά
}* Object;

// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
typedef struct state_info {
	Object jet;						// πληροφορίες για το αεροσκάφος
	Object missile;					// πληροφορίες για τον πύραυλο
	bool playing;					// true αν το παιχνίδι είναι ενεργό (false μετά από game over)
	bool paused;					// true αν το παιχνίδι είναι paused
	int score;						// το τρέχον σκορ
	int hearts;
	bool hit;
	time_t invis_t_start;
	float camera_x;
	float camera_y;
}* StateInfo;

// Πληροφορίες για το ποια πλήκτρα είναι πατημένα
typedef struct key_state {
	bool up;						// true αν το αντίστοιχο πλήκτρο είναι πατημένο
	bool down;
	bool left;
	bool right;
	bool space;
	bool enter;
	bool n;
	bool p;
}* KeyState;

// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Set objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};

// Η κατάσταση του παιχνιδιού (handle)
typedef struct state* State;


// Δημιουργεί και επιστρέφει ένα αντικείμενο
Object create_object(ObjectType, float, float, float, float);

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από y = start_y, και επεκτείνονται προς τα πάνω.

void add_objects(State, float);

// CompareFunc comparing objects (used for set)

int compare_objects(Pointer, Pointer);

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create();

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state);

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη y είναι ανάμεσα στο y_from και y_to.

List state_objects(State state, float y_from, float y_to);

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys);

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state);