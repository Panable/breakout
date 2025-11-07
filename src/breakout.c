#include <GLFW/glfw3.h>
#include <stdio.h>

int keys[1024] = {0};

void g_process_input(float deltaTime)
{
    if (keys[GLFW_KEY_A]) printf("Pressed A\n");
}

void g_update(float deltaTime)
{
}

void g_render()
{
}

void g_release()
{
}
