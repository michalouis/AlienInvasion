#include "raylib.h"
#include "ADTSet.h"

typedef enum{
    DRUGS, NO_MISSILE
} Effect;

typedef struct beam{
    Effect effect;
    bool move_right;
    Rectangle rect;
    Animation ring1;
    Vector2 ring1_pos;
    Animation ring2;
    Vector2 ring2_pos;
    Animation ring3;
    Vector2 ring3_pos;
    Animation ring4;
    Vector2 ring4_pos;
    Animation ring5;
    Vector2 ring5_pos;
}* Beam;

void beam_create(Set beams, float camera_y);

void beam_update(Set beams);