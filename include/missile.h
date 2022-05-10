#include "start_game.h"

typedef enum {
    P_MISSILE, H_MISSILE, W_MISSILE    // player missile, enemy missile
} MissileType;

typedef struct missile{
    MissileType type;
    Rectangle rect;
    bool upwards;
    bool right;
}* Missile;

int missile_comparefunc(Pointer a, Pointer b);

void missile_create(GameState, Rectangle, MissileType);

void missiles_update(GameState gamestate);