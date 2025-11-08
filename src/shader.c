#include "shader.h"
#include <glad/glad.h>
#include <stdio.h>
#include <string.h>

unsigned int shdr_check_compile_errors(unsigned int id, const char* type);

void shdr_use(unsigned int id)
{
    glUseProgram(id);
}

Shader shdr_compile(const char* v_src, const char* f_src, const char* g_src)
{
    /* --------- Compile Shader --------- */
    unsigned int v_id, f_id, g_id, prg_id;
    v_id = f_id = g_id = prg_id = 0;

    /* Vertex */
    v_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_id, 1, &v_src, NULL);
    glCompileShader(v_id);
    if (shdr_check_compile_errors(v_id, "VERTEX")) goto cleanup;

    /* Fragment */
    f_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_id, 1, &f_src, NULL);
    glCompileShader(f_id);
    if(shdr_check_compile_errors(f_id, "FRAGMENT")) goto cleanup;

    /* Geometry */
    if (g_src != NULL)
    {
        g_id = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(g_id, 1, &g_src, NULL);
        glCompileShader(g_id);
        if(shdr_check_compile_errors(g_id, "GEOMETRY")) goto cleanup;
    }

    /* Link Program */
    prg_id = glCreateProgram();

    glAttachShader(prg_id, v_id);
    glAttachShader(prg_id, f_id);
    if (g_src != NULL)
        glAttachShader(prg_id, g_id);

    glLinkProgram(prg_id);

    if(shdr_check_compile_errors(prg_id, "PROGRAM"))
    {
        glDeleteProgram(prg_id);
        prg_id = 0;
    }

cleanup:
    glDeleteShader(v_id);
    glDeleteShader(f_id);
    glDeleteShader(g_id);

    return prg_id;
}

unsigned int shdr_check_compile_errors(unsigned int id, const char* type)
{
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0)
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            printf("| ERROR::SHADER: Compile-time error: Type: %s\n%s\n -- --------------------------------------------------- --\n", type, infoLog);
            return 1;
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            printf("| ERROR::SHADER: Link-time error: Type: %s\n%s\n -- --------------------------------------------------- --\n", type, infoLog);
            return 1;
        }
    }

    return 0;
}

void shdr_set_float(const char* name, float value, Shader shdr)
{
    if (shdr == 0) {
        fprintf(stderr, "Error: Invalid shader program (0) for uniform '%s'\n", name);
        return;
    }
    
    int location = glGetUniformLocation(shdr, name);

    if (location == -1) {
        fprintf(stderr, "Warning: uniform '%s' not found in program %u\n", name, shdr);
        return;
    }

    glUniform1f(location, value);
}


void shdr_set_int(const char* name, int value, Shader shdr)
{
    if (shdr == 0) {
        fprintf(stderr, "Error: Invalid shader program (0) for uniform '%s'\n", name);
        return;
    }
    
    int location = glGetUniformLocation(shdr, name);

    if (location == -1) {
        fprintf(stderr, "Warning: uniform '%s' not found in program %u\n", name, shdr);
        return;
    }

    glUniform1i(location, value);
}

void shdr_set_vec2f(const char* name, vec2 value, Shader shdr)
{
    if (shdr == 0)
    {
        fprintf(stderr, "Error: Invalid shader program (0) for uniform '%s'\n", name);
        return;
    }
    
    int location = glGetUniformLocation(shdr, name);

    if (location == -1)
    {
        fprintf(stderr, "Warning: uniform '%s' not found in program %u\n", name, shdr);
        return;
    }

    glUniform2f(location, value[0], value[1]);
}

void shdr_set_vec3f(const char* name, vec3 value, Shader shdr)
{
    if (shdr == 0)
    {
        fprintf(stderr, "Error: Invalid shader program (0) for uniform '%s'\n", name);
        return;
    }
    
    int location = glGetUniformLocation(shdr, name);

    if (location == -1)
    {
        fprintf(stderr, "Warning: uniform '%s' not found in program %u\n", name, shdr);
        return;
    }

    glUniform3f(location, value[0], value[1], value[2]);
}

void shdr_set_vec4f(const char* name, vec4 value, Shader shdr)
{
    if (shdr == 0)
    {
        fprintf(stderr, "Error: Invalid shader program (0) for uniform '%s'\n", name);
        return;
    }
    
    int location = glGetUniformLocation(shdr, name);

    if (location == -1) {
        fprintf(stderr, "Warning: uniform '%s' not found in program %u\n", name, shdr);
        return;
    }

    glUniform4f(location, value[0], value[1], value[2], value[3]);
}

void shdr_set_mat4(const char* name, mat4 mat, Shader shdr)
{
    if (shdr == 0) {
        fprintf(stderr, "Error: Invalid shader program (0) for uniform '%s'\n", name);
        return;
    }
    
    int location = glGetUniformLocation(shdr, name);

    if (location == -1)
    {
        fprintf(stderr, "Warning: uniform '%s' not found in program %u\n", name, shdr);
        return;
    }

    glUniformMatrix4fv(location, 1, false, mat[0]);
}
