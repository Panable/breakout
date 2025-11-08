#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <glad/glad.h>

typedef unsigned int Texture;

typedef struct TextureSettings_s
{
    unsigned int Internal_Format; // format of texture object
    unsigned int Image_Format; // format of loaded image
    // texture configuration
    unsigned int Wrap_S; // wrapping mode on S axis
    unsigned int Wrap_T; // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
} TextureSettings;

extern const TextureSettings DefaultTextureSettings;

Texture tex_generate(unsigned int width, unsigned int height, unsigned char* data, TextureSettings settings);

void tex_bind(Texture tex);
void tex_unbind();

#endif /* end of include guard: TEXTURE_H */
