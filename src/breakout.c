#include <stdio.h>
#include <math.h>
#include "cglm/cglm.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int test()
{
    if (GLAD_GL_KHR_debug) {
        printf("Debugging KHR Works\n");
    } else if (GL_ARB_debug_output) {
        printf("Debug output supported\n");
    }
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
