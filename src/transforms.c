#include <stdio.h>
#define PL_MATH_IMPLEMENTATION
#include "pl_math.h"

int main(void)
{
    float test = 1.0f;
    printf("%.2f\n", test);
    mat3f matrix;

    vec3f test_vec = 
    {
        1.0f,
        2.0f,
        3.0f,
    };

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
