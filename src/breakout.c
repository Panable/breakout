#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

float r = 0.01f;
float g = 0.01f;
float b = 0.01f;

void input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float mul = 0.03f;
    switch (key)
    {
        case GLFW_KEY_Q:
        {
          r += mul;
          break;
        }
        case GLFW_KEY_W:
        {
          g += mul;
          break;
        }
        case GLFW_KEY_E:
        {
          b += mul;
          break;
        }
        default:
              break;
    }
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glewInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, input_callback);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
