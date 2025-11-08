#ifndef SHADER_H
#define SHADER_H
#include <cglm/cglm.h>

typedef unsigned int Shader;

void shdr_use(unsigned int id);
Shader shdr_compile(const char* v_src, const char* f_src, const char* g_src); // returns shader id.

/* Uniforms */
void shdr_set_float(const char* name, float value, Shader shdr);
void   shdr_set_int(const char* name, int   value, Shader shdr);
void shdr_set_vec2f(const char* name, vec2  value, Shader shdr);
void shdr_set_vec3f(const char* name, vec3  value, Shader shdr);
void shdr_set_vec4f(const char* name, vec4  value, Shader shdr);
void  shdr_set_mat4(const char* name, mat4    mat, Shader shdr);

#endif // SHADER_H
