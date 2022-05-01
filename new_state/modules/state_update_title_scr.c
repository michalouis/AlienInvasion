#include "raylib.h"
#include "interface.h"

void input(TitleScreen info, KeyState keys) {
    if (keys->up) info->button_selected--;
    
    if (info->button_selected < 0)
        info->button_selected = 0;

    if (keys->down) info->button_selected++;

    if (info->button_selected > 1)
        info->button_selected = 1; 

    if (info->button_selected == 0) {
        info->button1->color = YELLOW;
        info->button2->color = WHITE;
    } else {
        info->button1->color = WHITE;
        info->button2->color = YELLOW;
    }
}

void state_update_title_scr(TitleScreen info, KeyState keys) {
    input(info, keys);
}