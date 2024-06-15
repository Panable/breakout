#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static inline GLFWwindow* setup_glfw();
char* file_to_buffer();

int main()
{
    GLFWwindow* window = setup_glfw();
    assert(window && "GLFW FAILED TO INITIALISE");

    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    
    // Generate VBO
    unsigned int VBO;
    unsigned int VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Bind VBO to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Specify data format. */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    char infoLog[512];
    int success;

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = file_to_buffer("res/simple.vert");
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Compiling vertex shader failed %s\n", infoLog);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = file_to_buffer("res/simple.frag");
    printf(fragmentShaderSource);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Compiling fragment shader failed %s\n", infoLog);
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Linking failed %s", infoLog);
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexShaderSource);
    free(fragmentShaderSource);

    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/* Helper functions */

static inline GLFWwindow* setup_glfw()
{
    GLFWwindow* window;

    if (!glfwInit())
        return NULL;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Triangle!", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        fprintf(stderr, "%s\n", glewGetErrorString(err));
        return NULL;
    }

    return window;
}

char* file_to_buffer(const char* file_name)
{
    FILE* file = fopen(file_name, "r");
    
    /* calculate length of file */
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);
    char* buffer = malloc(length + 1);

    fread(buffer, 1, length, file);
    fclose(file);
    return buffer;
}
