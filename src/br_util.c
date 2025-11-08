#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stb_image.h"
#include "br_util.h"
char* read_file(const char* file_name)
{
    /* Get length of file */
    FILE* file = fopen(file_name, "rb");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", file_name);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long sz = ftell(file);
    rewind(file);

    char* buf = malloc(sz + 1);

    fread(buf, 1, sz, file);
    buf[sz] = '\0';
    
    fclose(file);

    return buf;
}

Shader load_shader_from_file(const char* vs, const char* fs, const char* gs)
{
    char* v_src =      read_file(vs);
    char* f_src =      read_file(fs);
    char* g_src = gs ? read_file(gs) : NULL;
    
    if (!v_src || !f_src || (gs && !g_src)) {
        fprintf(stderr, "Error: Failed to load shader files\n");
        free(v_src);
        free(f_src);
        free(g_src);
        return 0;
    }
    
    Shader shdr = shdr_compile(v_src, f_src, g_src);
    
    free(v_src); free(f_src); free(g_src);

    return shdr;
}

Texture load_texture_from_file(const char* file, bool alpha)
{
    TextureSettings settings = DefaultTextureSettings;
    Texture tex = 0;
    if (alpha)
    {
        settings.internal_format = GL_RGBA;
        settings.image_format    = GL_RGBA;
    }

    int width, height, num_channels;

    unsigned char* data = stbi_load(file, &width, &height, &num_channels, 0);

    tex = tex_generate(width, height, data, settings);

    stbi_image_free(data);
    return tex;
}
