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

void missile_create(GameState, Object, MissileType);

void missiles_update(GameState gamestate);