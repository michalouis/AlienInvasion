#include "game_screen.h"

typedef enum {
    P_MISSILE, LH_MISSILE, C_MISSILE, M_MISSILE    // player, longhorn, crab, mothership
} MissileType;

typedef struct missile{
    MissileType type;
    Rectangle rect;
    bool upwards;
    bool right;
}* Missile;

// CompareFunc for set used for missiles

int missile_comparefunc(Pointer a, Pointer b);

void missile_create(Game, Rectangle, MissileType);

// Update everything missile related

void missiles_update(Game, GameAssets);