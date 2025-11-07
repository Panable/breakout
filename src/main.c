#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    int x = 232;

    printf("%d\n", x);
    return 0;
}
