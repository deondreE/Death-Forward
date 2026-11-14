#include "mdash.h"
#include <string.h>

#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
// NOTE: https://clang.llvm.org/doxygen/xmmintrin_8h.html
#include <xmmintrin.h>
#define MD_USE_SSE 1
#else
#define MD_USE_SSE 0
#endif

Mat4 mat4_identity(void) {
  Mat4 res;
  memset(&res, 0, sizeof res);
  res.m[0][0] = 1.0f; res.m[1][1] = 1.0f;
  res.m[2][2] = 1.0f; res.m[3][3] = 1.0f;
  return res;
}

Mat4 mat4_mul(Mat4 a, Mat4 b) {
  Mat4 res;

  #if MD_USE_SSE
  /* column j of result = sum_k (column k of a) * b[j][k]*/
  __m128 a0 = _mm_loadu_ps(a.m[0]);
  __m128 a1 = _mm_loadu_ps(a.m[1]);
  __m128 a2 = _mm_loadu_ps(a.m[2]);
  __m128 a3 = _mm_loadu_ps(a.m[3]);
  for (int j = 0; j < 4; j++) {
    __m128 x = _mm_mul_ps(a0, _mm_set1_ps(b.m[j][0]));
    x = _mm_add_ps(x, _mm_mul_ps(a1, _mm_set1_ps(b.m[j][1])));
    x = _mm_add_ps(x, _mm_mul_ps(a2, _mm_set1_ps(b.m[j][2])));
    x = _mm_add_ps(x, _mm_mul_ps(a3, _mm_set1_ps(b.m[j][3])));
    _mm_storeu_ps(res.m[j], x);
  }
  
  #else
   for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      res.m[j][i] = a.m[0][i] * b.m[j][0] +
                    a.m[1][i] * b.m[j][1] +
                    a.m[2][i] * b.m[j][2] +
                    a.m[3][i] * b.m[j][3];
    }
  }
  #endif

  return res;
}

Mat4 mat4_transpose(Mat4 m) {
  Mat4 r;

  #if MD_USE_SSE
  __m128 c0 = _mm_loadu_ps(m.m[0]);
  __m128 c1 = _mm_loadu_ps(m.m[1]);
  __m128 c2 = _mm_loadu_ps(m.m[2]);
  __m128 c3 = _mm_loadu_ps(m.m[3]);
  _MM_TRANSPOSE4_PS(c0, c1, c2, c3);
  _mm_storeu_ps(r.m[0], c0);
  _mm_storeu_ps(r.m[1], c1);
  _mm_storeu_ps(r.m[2], c2);
  _mm_storeu_ps(r.m[3], c3);
  #else
  for (int c = 0; c < 4; c++)
    for (int row = 0; row < 4; row++)
      r.m[c][row] = m.m[row][c];
  #endif

  return r;
}

Vec4 mat4_mul_vec4(Mat4 m, Vec4 v) {
  Vec4 r;
  #if MD_USE_SSE
  __m128 x = _mm_mul_ps(_mm_loadu_ps(m.m[0]), _mm_set1_ps(v.x));
  x = _mm_add_ps(x, _mm_mul_ps(_mm_loadu_ps(m.m[1]), _mm_set1_ps(v.y)));
  x = _mm_add_ps(x, _mm_mul_ps(_mm_loadu_ps(m.m[2]), _mm_set1_ps(v.z)));
  x = _mm_add_ps(x, _mm_mul_ps(_mm_loadu_ps(m.m[3]), _mm_set1_ps(v.w)));
  _mm_storeu_ps(&r.x, x);
  #else
  r.x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w;
  r.y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w;
  r.z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w;
  r.w = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w;
  #endif
  
  return r;
}

Mat4 mat4_translate(Vec3 t) {
  Mat4 res = mat4_identity();
  res.m[3][0] = t.x;
  res.m[3][1] = t.y;
  res.m[3][2] = t.z;
  return res;
}

Mat4 mat4_scale(Vec3 s) {
  Mat4 r = mat4_identity();
  r.m[0][0] = s.x;
  r.m[1][1] = s.y;
  r.m[2][2] = s.z;
  return r;
}

Mat4 mat4_rotate_x(float angle) {
  Mat4 res = mat4_identity();
  float c = cosf(angle);
  float s = sinf(angle);
  res.m[1][1] = c; res.m[1][2] = s;
  res.m[2][1] = -s; res.m[2][2] = c;
  return res;
}

Mat4 mat4_rotate_y(float angle) {
  Mat4 res = mat4_identity();
  float c = cosf(angle);
  float s = sinf(angle);
  res.m[0][0] = c; res.m[0][2] = -s;
  res.m[2][0] = s; res.m[2][2] = c;
  return res;
} 

Mat4 mat4_rotate_z(float angle) {
  Mat4 res = mat4_identity();
  float c = cosf(angle);
  float s = sinf(angle);
  res.m[0][0] = c; res.m[0][1] = s;
  res.m[1][0] = -s; res.m[1][1] = c;
  return res;
}

Mat4 mat4_perspective(float fov, float aspect, float near, float far) {
  float t = tanf(fov / 2.0f);
  Mat4 res;
  memset(&res, 0, sizeof res);
  res.m[0][0] = 1.0f / (aspect * t);
  res.m[1][1] = 1.0f / t;
  res.m[2][2] = -(far + near) / (far - near);
  res.m[2][3] = -1.0f;
  res.m[3][2] = -(2.0f * far * near) / (far - near);
  return res;
}

Mat4 mat4_ortho(float l, float r_, float b, float t, float znear, float zfar) {
  Mat4 r = mat4_identity();
  r.m[0][0] = 2.0f / (r_ - l);
  r.m[1][1] = 2.0f / (t - b);
  r.m[2][2] = -2.0f / (zfar - znear);
  r.m[3][0] = -(r_ + l) / (r_ - l);
  r.m[3][1] = -(t + b) / (t - b);
  r.m[3][2] = -(zfar + znear) / (zfar - znear);
  return r;
}

Mat4 mat4_look_at(Vec3 eye, Vec3 center, Vec3 up) {
  Vec3 f = vec3_normalize(vec3_sub(center, eye));
  Vec3 s = vec3_normalize(vec3_cross(f, up));
  Vec3 u = vec3_cross(s, f);

  Mat4 r = mat4_identity();
  r.m[0][0] = s.x;  r.m[1][0] = s.y;  r.m[2][0] = s.z;
  r.m[0][1] = u.x;  r.m[1][1] = u.y;  r.m[2][1] = u.z;
  r.m[0][2] = -f.x; r.m[1][2] = -f.y; r.m[2][2] = -f.z;
  r.m[3][0] = -vec3_dot(s, eye);
  r.m[3][1] = -vec3_dot(u, eye);
  r.m[3][2] =  vec3_dot(f, eye);
  return r;
}
