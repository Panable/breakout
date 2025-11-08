#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stb_image.h"
#include "util.h"
char* read_file(const char* file_name)
{
    /* Get length of file */
    FILE* file = fopen(file_name, "rb");
    fseek(file, 0, SEEK_END);
    long sz = ftell(file);

    char* buf = malloc(sz + 1);

    fread(buf, 1, sz, file);
    buf[sz] = '\0';

    return buf;
}

Shader load_shader_from_file(const char* vs, const char* fs, const char* gs)
{
    char* v_src =      read_file(vs);
    char* f_src =      read_file(fs);
    char* g_src = gs ? read_file(gs) : NULL;
    
    Shader shdr = shdr_compile(g_src, f_src, g_src);
    
    free(v_src); free(f_src); free(g_src);

    return shdr;
}

Texture load_texture_from_file(const char* file, bool alpha)
{
    TextureSettings settings = DefaultTextureSettings;
    Texture tex = 0;
    if (alpha)
    {
        settings.Internal_Format = GL_RGBA;
        settings.Image_Format    = GL_RGBA;
    }

    int width, height, num_channels;

    unsigned char* data = stbi_load(file, &width, &height, &num_channels, 0);

    tex = tex_generate(width, height, data, settings);

    stbi_image_free(data);
    return tex;
}
