// #include "state.h"
#include "draw_related_funcs.h"

#include <stdlib.h>

TextInfo create_text(char* content, Vector2 pos, int size, Color color) {
    TextInfo text = malloc(sizeof(*text));

    text->text = content;
    text->pos.x = pos.x - MeasureText(content, size) /2;
    text->pos.y = pos.y;
    text->fontSize = size;
    text->color = color;

    return text;
}

TextureInfo create_texture_info(Vector2 pos, Rectangle rect, Color color) {
    TextureInfo texture_info = malloc(sizeof(*texture_info));

    texture_info->pos = pos;
    texture_info->rect = rect;
    texture_info->color = WHITE;

    return texture_info;
}

AnimationInfo create_animation_info(Texture texture, Vector2 pos, int frames) {
    AnimationInfo anim_info = malloc(sizeof(*anim_info));

    anim_info->pos = pos;
    anim_info->frameWidth = (float)(texture.width / frames);
    anim_info->maxFrames = (int)(texture.width / (int)anim_info->frameWidth);
    anim_info->timer = 0.0;
    anim_info->curr_frame = 0;
    
    return anim_info;
}

Animation create_animation(Texture texture, Vector2 pos, int frames) {
	Animation anim = malloc(sizeof(*anim));

	anim->texture = texture;
	anim->info = create_animation_info(anim->texture, pos, frames);

	return anim;
}

void animate(Animation anim, Vector2 pos, float change_frame_t, bool loop) {
	if (loop || anim->info->curr_frame != anim->info->maxFrames - 1) {
		anim->info->timer += GetFrameTime();

		if (anim->info->timer >= change_frame_t) {
			anim->info->timer = 0.0;
			anim->info->curr_frame++;
		}

		anim->info->curr_frame = anim->info->curr_frame % anim->info->maxFrames;
	}

	Rectangle texture_rec = {
			(anim->info->frameWidth * anim->info->curr_frame),
			0,
			anim->info->frameWidth,
			anim->texture.height
		};
	
	DrawTextureRec(
        anim->texture,
        texture_rec,
        pos,
        RAYWHITE
    );
}