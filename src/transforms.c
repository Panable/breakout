#include <stdio.h>
#define PL_MATH_IMPLEMENTATION
#include "pl_math.h"

int main(void)
{
    float test = 1.0f;
    printf("%.2f\n", test);
    mat3f matrix;

    mat3f a =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    mat3f b =
    {
        {10, 11, 12},
        {13, 14, 15},
        {16, 17, 9},
    };
    plm_mat3f_identity(matrix);
    plm_mat3f_mul(a, b, matrix);
    plm_mat3f_dump(matrix);
    return 0;
}
