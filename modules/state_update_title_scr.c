#include "raylib.h"
#include "interface.h"
#include "state.h"

void input1(TitleScreen info, KeyState keys) {
    if (keys->up) info->button_selected--;
    
    if (info->button_selected < 0)
        info->button_selected = 0;

    if (keys->down) info->button_selected++;

    if (info->button_selected > 1)
        info->button_selected = 1; 

    if (keys->enter) {
        if (info->button_selected == 0)
            info->button1->pressed = true;
    }

    if (info->button_selected == 0) {
        info->button1->texture_info->color = YELLOW;
        info->button2->texture_info->color = WHITE;
    } else {
        info->button1->texture_info->color = WHITE;
        info->button2->texture_info->color = YELLOW;
    }
}

void state_update_title_scr(TitleScreen info, KeyState keys) {
    input1(info, keys);
}