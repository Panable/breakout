#include <stdio.h>
#include <math.h>
#include "cglm/cglm.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLenum length, const char* message, const void* userparam);

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

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAGS)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC)glDebugOutput, NULL);
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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

