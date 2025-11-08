#include "sprite_renderer.h"
#include "br_util.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int keys[1024] = {0};

enum TextureID 
{
    TEX_FACE,
    TEX_CNT
};

enum ShaderID
{
    SHDR_FACE,
    SHDR_CNT
};

SpriteRenderer sr;

Shader shaders[SHDR_CNT] = {0};
Texture textures[TEX_CNT] = {0};

void g_init()
{
    shaders[SHDR_FACE] = load_shader_from_file("shaders/sprite_v.glsl", "shaders/sprite_f.glsl", NULL);

    mat4 projection = {0};
    glm_ortho(0.0f, SCR_WIDTH, SCR_HEIGHT, 0.0f, -1.0f, 1.0f, projection);

    shdr_use(shaders[SHDR_FACE]);
    shdr_set_int("image", 0, shaders[SHDR_FACE]);
    shdr_set_mat4("projection", projection, shaders[SHDR_FACE]);

    sr = sr_create(shaders[SHDR_FACE]);

    textures[TEX_FACE] = load_texture_from_file("res/awesomeface.png", true);
}

void g_process_input(float deltaTime)
{
    if (keys[GLFW_KEY_A]) printf("Pressed A\n");
}

void g_update(float deltaTime)
{
}

void g_render()
{
    vec2 pos = {200.0f, 200.0f};
    vec2 size = {300.0f, 400.0f};
    float rot = 45.0f;
    vec3 col = {0.0f, 1.0f, 0.0f};

    sr_draw_sprite(textures[TEX_FACE], pos, size, rot, col, sr);
}

void g_release()
{
}
