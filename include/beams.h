#include "raylib.h"
#include "ADTSet.h"

typedef struct beam{
    Rectangle rect;
    bool move_right;
}* Beam;

// Create beam

void beam_create(Set, float);

// Update anything beam related

void beam_update(Set, float);