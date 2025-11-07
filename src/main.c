#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCR_WIDTH 800 * 3
#define SCR_HEIGHT 600 * 3


void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLenum length, const char* message, const void* userparam);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

extern int keys[1024];
extern void g_process_input(float deltaTime);
extern void g_update(float deltaTime);
extern void g_render();
extern void g_release();

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Game!", NULL, NULL);

    if (window == NULL)
    {
        printf("window failed to create\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to init glad\n");
        return -1;
    }


    /* Debug callback */
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAGS)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC)glDebugOutput, NULL);
    }

    glfwSetKeyCallback(window, (GLFWkeyfun)key_callback);
    glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)framebuffer_size_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        g_process_input(deltaTime);
        g_update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        g_render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    g_release();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id,
                            GLenum severity,
                            GLenum length, 
                            const char* message,
                            const void* userparam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    
    printf("-----------\n");

    printf("Debug message (%d): %s\n", id, message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:               printf("Source: API");                break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     printf("Source: Window System");      break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   printf("Source: Shader Compiler");    break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       printf("Source: Third Party");        break;
        case GL_DEBUG_SOURCE_APPLICATION:       printf("Source: Application");        break;
        case GL_DEBUG_SOURCE_OTHER:             printf("Source: Other");              break;
    } printf("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               printf("Type: Error");                break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour");  break;
        case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability");          break;
        case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance");          break;
        case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker");               break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group");           break;
        case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group");            break;
        case GL_DEBUG_TYPE_OTHER:               printf("Type: Pop Group");            break;
    } printf("\n");

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:            printf("Severity: High");             break;
        case GL_DEBUG_SEVERITY_MEDIUM:          printf("Severity: Medium");             break;
        case GL_DEBUG_SEVERITY_LOW:             printf("Severity: Low");             break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:    printf("Severity: Notification");             break;
    }
}

