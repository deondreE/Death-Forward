#include "mdash.h"
#include <string.h>

Mat4 mat4_identity(void) {
  Mat4 res = {0};
  res.m[0][0] = 1.0f; res.m[1][1] = 1.0f;
  res.m[2][2] = 1.0f; res.m[3][3] = 1.0f;
  return res;
}

Mat4 mat4_mul(Mat4 a, Mat4 b) {
  Mat4 res = {0};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        res.m[i][j] += a.m[i][k] * b.m[k][j];
      }
    }
  }

  return res;
}

Mat4 mat4_perspective(float fov, float aspect, float near, float far) {
  float t = tanf(fov / 2.0f);
  Mat4 res = {0};
  res.m[0][0] = 1.0f / (aspect * t);
  res.m[1][1] = 1.0f / t;
  res.m[2][2] = -(far + near) / (far - near);
  res.m[2][3] = -(2.0f * far * near) / (far - near);
  res.m[3][2] = -1.0f;
  return res;
}

Mat4 mat4_translate(Vec3 t) {
  Mat4 res = mat4_identity();
  res.m[0][3] = t.x;
  res.m[1][3] = t.y;
  res.m[2][3] = t.z;
  return res;
}

Mat4 mat4_rotate_z(float angle) {
  Mat4 res = mat4_identity();
  float c = cosf(angle);
  float s = sinf(angle);
  res.m[0][0] = c; res.m[0][1] = -s;
  res.m[1][0] = s; res.m[1][1] = c;
  return res;
}
