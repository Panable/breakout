#ifndef PL_MATH_H
#define PL_MATH_H
#define PL_CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

/* Vectors */
typedef float vec2f[2];
typedef float vec3f[3];
typedef float vec4f[4];

/* Matrices */
typedef vec2f mat2f[2];
typedef vec3f mat3f[3];
typedef vec4f mat4f[4];

void plm_mat2f_identity(mat2f mat);
void plm_mat3f_identity(mat3f mat);
void plm_mat4f_identity(mat4f mat);

void plm_mat3f_mul(mat3f a, mat3f b, mat3f dest);

void plm_mat3f_dump(mat3f mat);
/* Column Major Ordering */
#ifdef PL_MATH_IMPLEMENTATION

void plm_mat2f_identity(mat2f mat)
{
}

void plm_mat3f_identity(mat3f mat)
{
    mat[0][0] = 1.0f;    mat[0][1] = 0.0f;    mat[0][2] = 0.0f;
    mat[1][0] = 0.0f;    mat[1][1] = 1.0f;    mat[1][2] = 0.0f;
    mat[2][0] = 0.0f;    mat[2][1] = 0.0f;    mat[2][2] = 1.0f;
}

void plm_mat4f_identity(mat4f mat)
{
}

void plm_mat3f_mul(mat3f a, mat3f b, mat3f dest)
{
    float a00 = a[0][0], a01 = a[0][1], a02 = a[0][2],
          a10 = a[1][0], a11 = a[1][1], a12 = a[1][2],
          a20 = a[2][0], a01 = a[2][1], a22 = a[2][2];

    float b00 = b[0][0], b01 = b[0][1], b02 = b[0][2],
          b10 = b[1][0], b11 = b[1][1], b12 = b[1][2],
          b20 = b[2][0], b01 = b[2][1], b22 = b[2][2];
}

void plm_mat3f_dump(mat3f mat)
{
    printf("%.2f, %.2f, %.2f\n", mat[0][0], mat[0][1], mat[0][2]);    
    printf("%.2f, %.2f, %.2f\n", mat[1][0], mat[1][1], mat[1][2]);    
    printf("%.2f, %.2f, %.2f\n", mat[2][0], mat[2][1], mat[2][2]);    
}

#endif // PL_MATH_IMPLEMENTATION

#endif PL_MATH_H // PL_MATH_H
