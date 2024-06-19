#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static inline unsigned int gen_shader(const char* vs, const char* fs);
static inline GLFWwindow* setup_glfw();
char* file_to_buffer();

int main(void)
{
    GLFWwindow* window = setup_glfw();

    /*             0
                 (0, 1)
                   /\
                  /  \
         1       / A  \      2
    (-0.5, 0)   /______\ (0.5, 0)
               /\      /\
              /  \    /  \
             / B  \  /  C \
            /______\/______\  
   (-1,-1.)      (0, -1)    (1, -1)           
         3           4          5
    */

    float vertices[] = {
    /* (  VERTEX  ), (    COLOUR    ) */
        0.0f,  1.0f, 1.0f, 0.0f, 0.0f, // 0
       -0.5f,  0.0f, 0.5f, 0.5f, 0.0f, // 1
        0.5f,  0.0f, 0.5f, 0.0f, 0.5f, // 2
       -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 3
        0.0f, -1.0f, 0.0f, 0.5f, 0.5f, // 4
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // 5
    };

    unsigned int elements[] = {
        0, 1, 2,    // A
        3, 4, 1,    // B
        4, 2, 5     // C
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

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    unsigned int shader = gen_shader("res/triforce/simple.vert", "res/triforce/simple.frag");
    glBindVertexArray(0);

    /* Main Loop */

    // long lastTime = 0;
    int additive = glGetUniformLocation(shader, "additive");

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        // double deltaTime = now - lastTime;
        float interpCol = (sinf(now) / 2.0f) + 0.5f;
        glfwPollEvents();

        float bg = 24.0f/255; // dark
        bg *= interpCol;
        glClearColor(bg, bg, bg, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shader);

        glUniform1f(additive, interpCol);
    
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)0);
        glfwSwapBuffers(window);
        // clastTime = now;
    }

    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

static inline GLFWwindow* setup_glfw()
{
    GLFWwindow* window;

    if (!glfwInit())
        assert(false && "GLFW failed to initialise");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "TRIFORCE!~", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        assert(false && "Failed to create window");
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        fprintf(stderr, "%s\n", glewGetErrorString(err));
        assert(false && "GLEW failed to initialise");
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

// convert file to string
char* file_to_buffer(const char* file_name)
{
    /* Open file */
    FILE* file = fopen(file_name, "r");
    assert(file);
    
    /* calculate length of file */
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);                      // Rewind pointer
    char* buffer = malloc(length + 1); // allocate space for file length + nullchar

    fread(buffer, 1, length, file);    // Read the file into buffer
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

static inline unsigned int gen_shader(const char* vs, const char* fs)
{
    /* Compile vertex shader */
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = file_to_buffer(vs);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    /* Error checking */
    char infoLog[512];
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Compiling vertex shader failed %s\n", infoLog);
    }

    /* Compile fragment shader */
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = file_to_buffer(fs);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /* Error Checking */
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Compiling fragment shader failed %s\n", infoLog);
    }

    /* Compile shader program */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* Error Checking */
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Linking failed %s", infoLog);
    }

   /* Delete the shaders (already in program) */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free((char*)vertexShaderSource);
    free((char*)fragmentShaderSource);

    return shaderProgram;
}

static inline void glCheckError()
{
    GLenum err = glGetError();
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        fprintf(stderr, "[OpenGL Error] ");
        switch (err) {
        case GL_INVALID_ENUM:
            fprintf(stderr, "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.");
            break;
        case GL_INVALID_VALUE:
            fprintf(stderr, "GL_INVALID_OPERATION : A numeric argument is out of range.");
            break;
        case GL_INVALID_OPERATION:
            fprintf(stderr, "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            fprintf(stderr, "GL_INVALID_FRAMEBUFFER_OPERATION : The framebuffer object is not complete.");
            break;
        case GL_OUT_OF_MEMORY:
            fprintf(stderr, "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.");
            break;
        case GL_STACK_UNDERFLOW:
            fprintf(stderr, "GL_STACK_UNDERFLOW : An attempt has been made to perform an operation that would cause an internal stack to underflow.");
            break;
        case GL_STACK_OVERFLOW:
            fprintf(stderr, "GL_STACK_OVERFLOW : An attempt has been made to perform an operation that would cause an internal stack to overflow.");
            break;
        default:
            fprintf(stderr, "Unrecognized error, %d", err);
        }
    }
}
