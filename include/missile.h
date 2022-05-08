#include "start_game.h"

typedef enum {
    P_MISSILE, E_MISSILE    // player missile, enemy missile
} MissileType;

typedef struct missile{
    MissileType type;
    Rectangle rect;
}* Missile;

void missile_create(GameState, Object, MissileType);

void missiles_update(GameState gamestate);