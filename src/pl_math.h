/* Column Major Ordering Matrix Library */
#ifndef PL_MATH_H
#define PL_MATH_H

#include <math.h>

#define PLM_CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#define PI 3.14159265358979323846f
#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)

/* Vectors */
typedef float vec3f[3];
typedef float vec2f[3];

/* Matrices */
typedef vec3f mat3f[3];

/* Vector Operations */
void plm_vec2f_scale(vec2f vec, vec2f amt);
void plm_vec2f_translate(vec2f vec, vec2f amt);
void plm_vec2f_rotate(vec2f vec, float rad);

void plm_mat3f_identity(mat3f mat);

void plm_mat3f_mul(mat3f a, mat3f b, mat3f dest);
void plm_mat3f_add(mat3f a, mat3f b, mat3f dest);

void plm_mat3f_dump(mat3f mat);


#ifdef PL_MATH_IMPLEMENTATION

void plm_vec2f_scale(vec2f vec, vec2f amt, vec2f dest)
{
    vec3f scratch = {
        vec[0],
        vec[1],
        1,
    };
}

void plm_vec2f_translate(vec2f vec, vec2f amt)
{
    
}

void plm_vec2f_rotate(vec2f vec, float rad)
{
}

void plm_mat3f_identity(mat3f mat)
{
    mat[0][0] = 1.0f;    mat[0][1] = 0.0f;    mat[0][2] = 0.0f;
    mat[1][0] = 0.0f;    mat[1][1] = 1.0f;    mat[1][2] = 0.0f;
    mat[2][0] = 0.0f;    mat[2][1] = 0.0f;    mat[2][2] = 1.0f;
}

void plm_mat3f_mul(mat3f a, mat3f b, mat3f dest)
{
    float a00 = a[0][0], a01 = a[0][1], a02 = a[0][2],
          a10 = a[1][0], a11 = a[1][1], a12 = a[1][2],
          a20 = a[2][0], a21 = a[2][1], a22 = a[2][2];

    float b00 = b[0][0], b01 = b[0][1], b02 = b[0][2],
          b10 = b[1][0], b11 = b[1][1], b12 = b[1][2],
          b20 = b[2][0], b21 = b[2][1], b22 = b[2][2];

    dest[0][0] = b00 * a00 + b01 * a10 + b02 * a20;
    dest[0][1] = b00 * a01 + b01 * a11 + b02 * a21;
    dest[0][2] = b00 * a02 + b01 * a12 + b02 * a22;

    dest[1][0] = b10 * a00 + b11 * a10 + b12 * a20;
    dest[1][1] = b10 * a01 + b11 * a11 + b12 * a21;
    dest[1][2] = b10 * a02 + b11 * a12 + b12 * a22;

    dest[2][0] = b20 * a00 + b21 * a10 + b22 * a20;
    dest[2][1] = b20 * a01 + b21 * a11 + b22 * a21;
    dest[2][2] = b20 * a02 + b21 * a12 + b22 * a22;
}

void plm_mat3f_dump(mat3f mat)
{
    printf("\t%.2f, %.2f, %.2f\n", mat[0][0], mat[0][1], mat[0][2]);    
    printf("\t%.2f, %.2f, %.2f\n", mat[1][0], mat[1][1], mat[1][2]);    
    printf("\t%.2f, %.2f, %.2f\n", mat[2][0], mat[2][1], mat[2][2]);    
}

#endif // PL_MATH_IMPLEMENTATION

#endif PL_MATH_H // PL_MATH_H
