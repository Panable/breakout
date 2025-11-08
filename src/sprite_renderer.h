#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "shader.h"
#include "texture.h"

typedef struct 
{
    Shader shader;
    unsigned int VAO;
} SpriteRenderer;


void sr_draw_sprite(Texture tex, vec2 pos, vec2 size, float rotate, vec3 color, SpriteRenderer sr);
SpriteRenderer sr_create(Shader shader);

#endif /* end of include guard: SPRITE_RENDERER_H */
