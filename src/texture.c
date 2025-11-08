#include "texture.h"

const TextureSettings DefaultTextureSettings = {.Internal_Format = GL_RGB, 
                                                .Image_Format    = GL_RGB, 
                                                .Wrap_S          = GL_REPEAT,
                                                .Wrap_T          = GL_REPEAT,
                                                .Filter_Min      = GL_LINEAR, 
                                                .Filter_Max      = GL_LINEAR};

Texture tex_generate(unsigned int width, unsigned int height, unsigned char* data, TextureSettings settings)
{
    Texture tex;
    glGenTextures(1, &tex);

    tex_bind(tex);
    glTexImage2D(GL_TEXTURE_2D, 0, settings.Internal_Format, width, height, 0, settings.Image_Format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     settings.Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     settings.Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.Filter_Max);

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
