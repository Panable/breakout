#include <stdio.h>
#define PL_MATH_IMPLEMENTATION
#include "pl_math.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

char* read_file(const char* file_name);
int main(void)
{
    read_file("res/transforms/simple.frag");
    GLFWwindow* window;
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "Hello World", 0, 0);
    glfwMakeContextCurrent(window);


    GLenum err = glewInit();
    if (err)
    {
        fprintf(stderr, "Error initializing glad\n");
    }


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.1f, 0.1f, 0.1f, 0.1f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


char* read_file(const char* file_name)
{
    FILE* file;

    file = fopen(file_name, "r");

    /* Get file length */
    fseek(file, 0, SEEK_END);
    long sz = ftell(file);
    rewind(file);

    printf("The size of the file is %zu\n", sz);
    return 0;
}

int testing_transforms(void)
{
   vec2f test_vec = 
   {
       1.0f,
       2.0f,
   };

   vec2f scaled = {0};

   vec2f scale_factor = {1.0f, 1.0f};

   plm_vec2f_scale(test_vec, (vec2f){2.0f, 2.0f}, scaled);
   printf("translated vector:\n");
   plm_vec2f_dump(scaled);

   return 0;
   float test = 1.0f;
   printf("%.2f\n", test);
   mat3f matrix;

   mat3f a =
   {
       {1, 4, 7},
       {2, 5, 8},
       {3, 6, 9},
   };

   mat3f b =
   {
       {10, 13, 16},
       {11, 14, 17},
       {12, 15, 18},
   };

   printf("Matrix A:\n");
   plm_mat3f_dump(a);
   printf("Matrix B:\n");
   plm_mat3f_dump(b);

   plm_mat3f_identity(matrix);
   plm_mat3f_mul(a, b, matrix);
   plm_mat3f_dump(matrix);

   float* mat_data = (float*)matrix;

   printf("The first data block is %f", *(mat_data + 1));

   return 0;

}
