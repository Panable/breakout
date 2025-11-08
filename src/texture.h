#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <glad/glad.h>

typedef unsigned int Texture;

typedef struct TextureSettings_s
{
    unsigned int internal_format; // format of texture object
    unsigned int image_format; // format of loaded image
    // texture configuration
    unsigned int wrap_s; // wrapping mode on S axis
    unsigned int wrap_t; // wrapping mode on T axis
    unsigned int filter_min; // filtering mode if texture pixels < screen pixels
    unsigned int filter_max; // filtering mode if texture pixels > screen pixels
} TextureSettings;

extern const TextureSettings DefaultTextureSettings;

Texture tex_generate(unsigned int width, unsigned int height, unsigned char* data, TextureSettings settings);

void tex_bind(Texture tex);
void tex_unbind();

#endif /* end of include guard: TEXTURE_H */
