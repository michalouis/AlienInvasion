// #include "state.h"
#include "draw_related_funcs.h"

#include <stdlib.h>

TextInfo create_text(char* content, Vector2 pos, bool center_pos, int size, Color color) {
    TextInfo text = malloc(sizeof(*text));

    text->text = content;

    if (center_pos) {
        text->pos.x = pos.x - MeasureText(content, size) /2;
        text->pos.y = pos.y;
    } else {
        text->pos = pos;
    }

    text->fontSize = size;
    text->color = color;

    return text;
}

void destroy_text(TextInfo text) {
    free(text);
}

TextureInfo create_texture_info(Vector2 pos, bool center_pos, Rectangle rect, Color color) {
    TextureInfo texture_info = malloc(sizeof(*texture_info));

    if (center_pos) {
        texture_info->pos.x = pos.x - rect.width / 2;
        texture_info->pos.y = pos.y;
    } else {
        texture_info->pos = pos;
    }

    texture_info->rect = rect;
    texture_info->color = WHITE;

    return texture_info;
}

void destroy_texture_info(TextureInfo texture_info) {
    free(texture_info);
}

Animation create_animation(Texture texture, Vector2 pos, int frames) {
	Animation anim = malloc(sizeof(*anim));

	anim->texture = texture;

    AnimationInfo anim_info = malloc(sizeof(*anim_info));
    anim_info->pos = pos;
    anim_info->frameWidth = (float)(texture.width / frames);
    anim_info->maxFrames = (int)(texture.width / (int)anim_info->frameWidth);
    anim_info->timer = 0.0;
    anim_info->curr_frame = 0;

    anim->info = anim_info;

	return anim;
}

void destroy_animation(Animation anim) {
    UnloadTexture(anim->texture);
    free(anim->info);

    free(anim);
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