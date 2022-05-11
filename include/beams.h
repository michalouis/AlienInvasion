#include "raylib.h"
#include "ADTSet.h"

typedef enum{
    DRUGS, NO_MISSILE
} Effect;

typedef struct beam{
    Effect effect;
    bool move_right;
    Rectangle rect;
    Vector2 ring1;
    Vector2 ring2;
    Vector2 ring3;
    Vector2 ring4;
    Vector2 ring5;
}* Beam;

void beam_create(Set beams, float camera_y);

void beam_update(Set beams);