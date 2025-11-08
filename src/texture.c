#include "texture.h"

const TextureSettings DefaultTextureSettings = {.internal_format = GL_RGB, 
                                                .image_format    = GL_RGB, 
                                                .wrap_s          = GL_REPEAT,
                                                .wrap_t          = GL_REPEAT,
                                                .filter_min      = GL_LINEAR, 
                                                .filter_max      = GL_LINEAR};

Texture tex_generate(unsigned int width, unsigned int height, unsigned char* data, TextureSettings settings)
{
    Texture tex;
    glGenTextures(1, &tex);

    tex_bind(tex);
    glTexImage2D(GL_TEXTURE_2D, 0, settings.internal_format, width, height, 0, settings.image_format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     settings.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     settings.wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filter_max);

    tex_unbind();
    return tex;
}

void tex_bind(Texture tex)
{
    glBindTexture(GL_TEXTURE_2D, tex);
}

void tex_unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
