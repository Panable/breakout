#include "sprite_renderer.h"

void sr_draw_sprite(Texture tex, vec2 pos, vec2 size, float rotate, vec3 color, SpriteRenderer sr)
{
    shdr_use(sr.shader);
    mat4 model = GLM_MAT4_IDENTITY_INIT;

    glm_translate(model, (vec3){pos[0], pos[1], 0.0f});
    glm_translate(model, (vec3){0.5f * size[0], 0.5f * size[1], 0.0f});
    glm_rotate(model, glm_rad(rotate), (vec3){0.0f, 0.0f, 1.0f});
    glm_translate(model, (vec3){-0.5f * size[0], -0.5f * size[1], 0.0f});
    glm_scale(model, (vec3){size[0], size[1], 1.0f});

    shdr_set_mat4("model", model, sr.shader);
    shdr_set_vec3f("spriteColor", color, sr.shader);


    glActiveTexture(GL_TEXTURE0);
    tex_bind(tex);
    glBindVertexArray(sr.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

SpriteRenderer sr_create(Shader shader)
{
    SpriteRenderer sr = {0};
    sr.shader = shader;
    unsigned int VBO;
    
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &sr.VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(sr.VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return sr;
}
