#include "jet.h"

#include <stdlib.h>

Jet jet_create(float x, float y, float width, float height) {
    Jet jet = malloc(sizeof(*jet));

    jet->rect.x = x;
    jet->rect.y = y;
    jet->rect.width = width;
    jet->rect.height = height;;
    jet->hearts = 3;
    jet->hit = false;
    jet->invis_t_start = 0;

    return jet;
}

Jet jet_reset(Jet jet, float x, float y) {
    jet->rect.x = x;
    jet->rect.y = y;

    jet->hearts = 3;
    jet->hit = false;
    jet->invis_t_start = 0;

    return jet;
}

void jet_destroy(Jet jet) {
    free(jet);
}

