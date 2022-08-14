#include "raylib.h"
#include "state.h"
#include "beams.h"
#include "ADTSet.h"
#include "draw_related_funcs.h"

#include <stdlib.h>

void beam_create(Set beams, Vector2 jet_pos) {
    Beam beam = malloc(sizeof(*beam));

    // Beam comes from the right/left (random)
    if (GetRandomValue(0,1))
        beam->move_right = true;
    else
        beam->move_right = false;

    // Spawn beam off screen, pos_y random
    int pos_y = jet_pos.y;
    int pos_x = beam->move_right ? -500 : SCREEN_W_G + 585;

    beam->rect = (Rectangle) {
        pos_x, pos_y,
        85, 20
    };
    
    set_insert(beams, beam);
}

// Update beam coordinates

static void beam_movement(Beam beam, float speed) {
    int pixels = beam->move_right ? +7 : -7;

    beam->rect.y -= 3 * speed;  // beam appears to move parallel to x axis
    beam->rect.x += pixels * speed;
}


// Destroy beam if it went from one side of the screen to the other

static bool beam_despawn(Set beams, Beam beam) {
    if (beam->move_right && beam->rect.x > SCREEN_W_G) {
        set_remove(beams, beam);
        return true;
    } else if (!beam->move_right && beam->rect.x + beam->rect.width < 0) {
        set_remove(beams, beam);
        return true;
    }

    return false;
}

void beam_update(Set beams, float speed) {
    for(SetNode node = set_first(beams);
		node != SET_EOF;
		node = set_next(beams, node)) {

        Beam beam = set_node_value(beams, node);

        beam_movement(beam, speed);
    }
    
    // If a beam is removed from set iterate set from the start
    // because the set arragment has changed
    for(SetNode node = set_first(beams);
		node != SET_EOF;
		node = set_next(beams, node)) {

        Beam beam = set_node_value(beams, node);

        if (beam_despawn(beams, beam)) {
            if (set_size(beams) != 0)
                node = set_first(beams);
            else
                break;
        }
    }
}

