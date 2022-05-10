#include "jet.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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

void jet_hit(Jet jet) {
    if (jet->hit && jet->invis_t_start == 0) {
        jet->hearts--;
        jet->invis_t_start = time(NULL);
    } 
    
    if (jet->hit && jet->invis_t_start) {
        time_t t_now = time(NULL);

		if(t_now > jet->invis_t_start + 5) {
			jet->invis_t_start = 0;
			jet->hit = false;
		}
    }
}

bool jet_gameover(Jet jet) {
    if (jet->hearts)
        return false;
    else
        return true;
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

