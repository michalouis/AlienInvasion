#include <stdlib.h>

#include "state.h"

// Αρχικοποιεί την κατάσταση το παιχνιδιού

static void state_init(State state) {
	state->score = 0;
	state->playing = true;

	state->character.width = 47;
	state->character.height = 39;
	state->character.x = 80;
	state->character.y = SCREEN_HEIGHT / 2 - state->character.height / 2;

	state->ball1.radius = 15;
	state->ball1.position.x = SCREEN_WIDTH / 3.;
	state->ball1.position.y = state->ball1.radius;

	state->ball2.radius = 30;
	state->ball2.position.x = 2 * SCREEN_WIDTH / 3.;
	state->ball2.position.y = state->ball2.radius;
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	State state = malloc(sizeof(*state));
	state_init(state);
	return state;
}

// Ενημερώνει την κατάσταση του παιχνιδιού για το επόμενο frame
//
// Για απλότητα, χρησιμοποιούμε απ'ευθείας τη συνάρτηση IsKeyDown(key) για
// έλεγχο των πλήκτρων. Μια καλύτερη υλοποίηση θα έπαιρνε την κατάσταση των
// πλήκτρων σαν όρισμα, ώστε να μπορούμε να δοκιμάσουμε τη συνάρτηση με
// κατάλληλα tests.
//
void state_update(State state) {
	state->game_over = false;		// το true διαρκεί μόνο 1 frame

	if (state->playing) {
		state->character.x += IsKeyDown(KEY_LEFT) ? 1 : 3;

		// wrap
		if (state->character.x > SCREEN_WIDTH) {
			state->character.x = 0;
			state->score++;
		}

		state->ball1.position.y = (int)(state->ball1.position.y + 5) % SCREEN_HEIGHT;
		state->ball2.position.y = (int)(SCREEN_HEIGHT + state->ball2.position.y - 7) % SCREEN_HEIGHT;

		if (IsKeyDown(KEY_UP))
			state->character.y -= 3;
		else
			state->character.y += 1;

		// έλεγχος για collisions
		if (state->character.y < 0 ||
			state->character.y > SCREEN_HEIGHT - state->character.height ||
			CheckCollisionCircleRec(state->ball1.position, state->ball1.radius, state->character) ||
			CheckCollisionCircleRec(state->ball2.position, state->ball2.radius, state->character)) {

			state->playing = false;
			state->game_over = true;
		}

	} else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
		state_init(state);		// επαναφορά στην αρχική κατάσταση
	}
}