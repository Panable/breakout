#ifndef BR_UTIL_H
#define BR_UTIL_H
#include "shader.h"
#include "texture.h"

char* read_file(const char* file_name);
Shader load_shader_from_file(const char* vs, const char* fs, const char* gs);
Texture load_texture_from_file(const char* file, bool alpha);

#endif /* end of include guard: UTIL_H */
