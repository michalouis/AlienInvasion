#include "draw_related_funcs.h"
#include <stdlib.h>

TextureInfo textureInfo_create(Vector2 pos, bool center_pos, Rectangle rect, Color color) {
    TextureInfo texture_info = malloc(sizeof(*texture_info));

    // Coordinates
    if (center_pos) {   //if we want the center of the texture to be on pos
        texture_info->pos.x = pos.x - rect.width / 2;
        texture_info->pos.y = pos.y;
    } else {
        texture_info->pos = pos;
    }

    // Rectangle & Color
    texture_info->rect = rect;
    texture_info->color = WHITE;

    return texture_info;
}

void textureInfo_destroy(TextureInfo texture_info) {
    free(texture_info);
}

Animation animation_create(Texture texture, int frames) {
	Animation anim = malloc(sizeof(*anim));

	anim->texture = texture;    //texture that contains animation's frames

    anim->frameWidth = (float)(texture.width / frames); // width of each frame
    anim->maxFrames = (int)(texture.width / (int)anim->frameWidth); // number of frames
    anim->timer = 0.0;  // counts time till frame needs to change
    anim->curr_frame = 0;   // what frame is currently drawn

	return anim;
}

bool animation_animate(Animation anim, Vector2 pos, float change_frame_t, Color color, bool loop) {

    // if all frames of the animation have been drawn, don't draw the animation again (dont loop)
    if (!loop && anim->curr_frame == anim->maxFrames - 1 && change_frame_t < anim->timer) {
        return false;
    }

    // updates the frame of the animation
    // depending on the animation's speed
	if (anim->timer >= change_frame_t) {
		anim->timer = 0.0;
		anim->curr_frame++;
	}

    // if all frames have been drawn go back
    // to the first frame and start over
	anim->curr_frame = anim->curr_frame % anim->maxFrames;

    // find coordinates & dimensions of current frame
	Rectangle texture_rec = {
		anim->frameWidth * anim->curr_frame,
		0,
		anim->frameWidth,
		anim->texture.height
	};
	
    // draw current frame
	DrawTextureRec(
        anim->texture,
        texture_rec,
        pos,
        color
    );

	anim->timer += GetFrameTime();  // update time till next frame is drawn
    return true;
}

bool animation_finished(Animation anim, float change_frame_t) {
    if (anim->curr_frame == anim->maxFrames - 1
        && change_frame_t < anim->timer)
        return true;

    return false;
}

void animation_reset(Animation anim) {
    anim->curr_frame = 0;   // go to first frame
    anim->timer = 0.0;      // reinitialize timer
}

void animation_destroy(Animation anim) {
    UnloadTexture(anim->texture);

    free(anim);
}