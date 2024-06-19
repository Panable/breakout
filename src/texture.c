#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdlib.h>
#include "stb_image.h"

unsigned int create_shader_program(const char* vs, const char* fs);
char* read_file(const char* file_name);
void window_resized(GLFWwindow* window, int width, int height);

int main(void)
{
    /*********************** SETUP *********************************/
    GLFWwindow* window;
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "Texture", NULL, NULL);
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err)
    {
        fprintf(stderr, "There seems to be a problem here\n");
    }

    glfwSetFramebufferSizeCallback(window, window_resized);
    /***************************************************************/

    /*

          0 (-0.5, 0.5) +--------------------------+ (0.5, 0.5) 1
                        |  x                       |
                        |     x                    |
                        |        x         B       |
                        |           x              |
                        |     A        x           |
                        |                 x        |
                        |                    x     |
                        |                       x  |
         2 (-0.5, -0.5) +--------------------------+ (0.5, -0.5) 3
                                                                    */

    float vertices[] = {
        -0.5f,  0.5f, // 0
         0.5f,  0.5f, // 1
        -0.5f, -0.5f, // 2
         0.5f, -0.5f, // 3
    };

    unsigned int elements[] = {
        0, 2, 3, // A
        0, 1, 3, // B
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* IMAGE */

    // Basic usage (see HDR discussion below for HDR usage):
    //    int x,y,n;
    //    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    //    // ... process data if not NULL ...
    //    // ... x = width, y = height, n = # 8-bit components per pixel ...
    //    // ... replace '0' with '1'..'4' to force that many components per pixel
    //    // ... but 'n' will always be the number that it would have been if you said 0
    //    stbi_image_free(data);

    int width, height, num_components;

    unsigned char* data = stbi_load("res/texture/texture.jpg", &width, &height, &num_components, 0);
    assert(data);

    printf("Image has been loaded.\nWidth: %d\nHeight:%d\nNumber of Components: %d", width, height, num_components);

    // Sampler2D in fragment shader
    // pass in texture id to vertex to pass to fragment
    // scaling on the axes, theres like gl_repeat and shit
    // NO, we pass texture coordinates i remember
    // glParametriv
    // mipmaps only for minifying NOT magnifying
    // texture(slot, coord) function fragment shader
    // Bind -> Active texture.

    /* Quickly compile shader */
    unsigned int sp = create_shader_program("res/texture/simple.vert", "res/texture/simple.frag");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.24f, 0.24f, 0.24f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(sp);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

unsigned int create_shader_program(const char* vs, const char* fs)
{
    const char* vs_src = read_file(vs);
    const char* fs_src = read_file(fs);

    unsigned int vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_id, 1, &vs_src, NULL);
    glCompileShader(vs_id);

    char info[512];
    int success;

    glGetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs_id, 512, NULL, info);
        printf("[ERROR] Compiling shader failed. %s\n", info);
    }

    unsigned int fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_id, 1, &fs_src, NULL);
    glCompileShader(fs_id);
    glGetShaderiv(fs_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs_id, 512, NULL, info);
        printf("[ERROR] Compiling shader failed. %s\n", info);
    }

    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vs_id);
    glAttachShader(shader, fs_id);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 512, NULL, info);
        printf("[ERROR] Linking shader failed. %s\n", info);
    }

    free((char*)vs_src);
    free((char*)fs_src);
    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
    assert(shader);
    return shader;
}

char* read_file(const char* file_name)
{
    /* calculate length of file */

    FILE* file = fopen(file_name, "r");
    assert(file);
    fseek(file, 0, SEEK_END);
    long sz = ftell(file);
    rewind(file);

    char* buf = malloc(sz + 1);

    fread(buf, 1, sz, file);
    buf[sz] = '\0';

    return buf;
}
void window_resized(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
