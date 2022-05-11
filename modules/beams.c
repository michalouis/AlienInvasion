#include "ADTSet.h"
#include "raylib.h"
#include "draw_related_funcs.h"
#include "state.h"
#include "beams.h"

#include <stdlib.h>

void beam_create(Set beams, float camera_y) {
    Beam beam = malloc(sizeof(*beam));

    beam->effect = GetRandomValue(DRUGS, NO_MISSILE);

    if (GetRandomValue(0,1))
        beam->move_right = true;
    else
        beam->move_right = false;

    int pos_y = camera_y + SCREEN_HEIGHT * ((float)GetRandomValue(1,3) / 4.0f);
    int pos_x = beam->move_right ? -100 : SCREEN_W_G + 100;

    beam->rect = (Rectangle) {
        pos_x,
        pos_y,
        85,
        20
    };

    pos_x = beam->rect.x;
    beam->ring1 = (Vector2) {
        pos_x + 17 * 0 , pos_y
    };

    beam->ring2 = (Vector2) {
        pos_x + 17 * 1 , pos_y
    };

    beam->ring3 = (Vector2) {
        pos_x + 17 * 2 , pos_y
    };

    beam->ring4 = (Vector2) {
        pos_x + 17 * 3 , pos_y
    };

    beam->ring5 = (Vector2) {
        pos_x + 17 * 4 , pos_y
    };
    
    set_insert(beams, beam);
}

void beam_movement(Beam beam) {
    int pixels = beam->move_right ? +8 : -8;

    beam->ring1.y -= 3;
    beam->ring2.y -= 3;
    beam->ring3.y -= 3;
    beam->ring4.y -= 3;
    beam->ring5.y -= 3;

    beam->ring1.x += pixels;
    beam->ring2.x += pixels;
    beam->ring3.x += pixels;
    beam->ring4.x += pixels;
    beam->ring5.x += pixels;
}

void beam_update(Set beams) {
    for(SetNode node = set_first(beams);
		node != SET_EOF;
		node = set_next(beams, node)) {

        Beam beam = set_node_value(beams, node);

        beam_movement(beam);
    }
}

