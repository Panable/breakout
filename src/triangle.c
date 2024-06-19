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

    /*                  3
                   0.0f, 0.5f
                       /\
                      /  \
                     /    \
                    /      \
                   /        \
                  /          \
                 /            \
          1     /______________\     2
    -0.5f, -0.5f                0.5f, -0.5f    */

    float vertices[] =
    {
        -0.5f, -0.5f, // 1
         0.5f, -0.5f, // 2
         0.0f,  0.5f, // 3
    };
    
    
    /* VBOs are used to 'buffer', objects to the GPU. I.e. send memory. */
    unsigned int VBO;

    /* 
     VAOs store everything needed to draw something (excluding shaders):
                          glEnableVertexAttribArray                      
                          glDisableVertexAttribArray                      
                          glVertexAttribPointer
                          glBindBuffer
     -------------------------------------------------------------------
     VAOs store buffer objects and the data layout.
     They can also store EBOs too!
   */

    unsigned int VAO;

    glGenBuffers(1, &VBO);      // Generate VBO ID
    glGenVertexArrays(1, &VAO); // Generate VAO ID

    glBindVertexArray(VAO);

    // Bind VBO to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // copy data to bound buffer (GL_ARRAY_BUFFER)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Specify data format. */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    // 0                 -> position (location = 0)
    // 2                 -> number of elements
    // GL_FLOAT          -> type of data
    // sizeof(float) * 3 -> size of single element / stride
    // (void*)0          -> offset to start at

    // Enable index 0 of vertex attribute
    glEnableVertexAttribArray(0);

    /* Unbind Objs */

    // Order only matters for ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    /* Compile vertex shader */
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = file_to_buffer("res/triangle/simple.vert");
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
    const char* fragmentShaderSource = file_to_buffer("res/triangle/simple.frag");
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

    while (!glfwWindowShouldClose(window))
    {
        /* Clear Window */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        
        glfwSwapBuffers(window); // Double buffering, swap
        glfwPollEvents();        // Poll inputs
    }

    glfwTerminate();
    return 0;
}

/* Helper functions */

// On resize
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    // Resize the viewport to match new width and height.
    glViewport(0, 0, width, height);
}

static inline GLFWwindow* setup_glfw()
{
    GLFWwindow* window; // window

    // Initialize glfw
    if (!glfwInit())
        return NULL;

    // Initialize opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    window = glfwCreateWindow(640, 480, "Triangle!", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return NULL;
    }

    // Makes the opengl context
    glfwMakeContextCurrent(window);

    /* Intiialize OpenGL function pointers */
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "%s\n", glewGetErrorString(err));
        return NULL;
    }

    // Setup resize callback
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
    buffer[length + 1] = '\0';
    fclose(file);
    return buffer;
}
