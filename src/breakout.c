#include <GLFW/glfw3.h>
#include <stdio.h>
int keys[1024] = {0};

extern void g_ProcessInput(float deltaTime)
{
    if (keys[GLFW_KEY_A]) printf("Pressed A\n");
}

extern void g_Update(float deltaTime)
{
}

extern void g_Render()
{
}

extern void g_Release()
{
}
