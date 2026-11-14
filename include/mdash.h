#ifndef M_DASH_H
#define M_DASH_H

#include <math.h>

typedef struct { float x, y; } Vec2;
typedef struct { float x, y,z; } Vec3;
typedef struct { float m[4][4]; } Mat4;


Mat4 mat4_identity(void);

Mat4 mat4_mul(Mat4 a, Mat4 b);

// @Todo: Ortho projection
Mat4 mat4_perspective(float fov, float aspect, float near, float far);

Mat4 mat4_translate(Vec3 translation);
Mat4 mat4_rotate_z(float angle_radians);


#endif
