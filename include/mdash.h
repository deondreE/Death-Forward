#ifndef M_DASH_H
#define M_DASH_H

#include <math.h>

#if defined(_MSC_VER)
#define MD_INLINE static __forceinline
#else
#define MD_INLINE static inline __attribute__((always_inline))
#endif

#define MD_PI 3.14159265358979323846f
#define MD_DEG2RAD (MD_PI / 180.0f)
#define MD_RAD2DEG (180.0f / MD_PI)

typedef struct { float x, y; } Vec2;
typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Vec4;
typedef struct { float m[4][4]; } Mat4;

// Scalar helpers
MD_INLINE float md_radians(float deg) { return deg * MD_DEG2RAD; }
MD_INLINE float md_degrees(float rad) { return rad * MD_RAD2DEG; }
MD_INLINE float md_minf(float a, float b) { return  a < b ? a : b; }
MD_INLINE float md_maxf(float a, float b) { return  a > b ? a : b; }
MD_INLINE float md_clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
MD_INLINE float md_lerpf(float a, float b, float t) { return a + (b-a) * t; } // linear across time.

// Vec2
MD_INLINE Vec2 vec2(float x, float y) { Vec2 r = {x,y}; return r; }
MD_INLINE Vec2 vec2_add(Vec2 a, Vec2 b) { return vec2(a.x + b.x, a.y + b.y); }
MD_INLINE Vec2 vec2_sub(Vec2 a, Vec2 b) { return vec2(a.x - b.x, a.y - b.y); }
MD_INLINE Vec2 vec2_scale(Vec2 a, float s) { return vec2(a.x * s, a.y * s); }
MD_INLINE float vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
MD_INLINE float vec2_length(Vec2 a) { return sqrtf(vec2_dot(a, a)); }

// Vec3
MD_INLINE Vec3 vec3(float x, float y, float z) { Vec3 r = {x,y,z}; return r; }
MD_INLINE Vec3 vec3_add(Vec3 a, Vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
MD_INLINE Vec3 vec3_sub(Vec3 a, Vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
MD_INLINE Vec3 vec3_scale(Vec3 a, float s) { return vec3(a.x * s, a.y * s, a.z * s); }
MD_INLINE float vec3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
MD_INLINE float vec3_length(Vec3 a) { return sqrtf(vec3_dot(a, a)); }

MD_INLINE Vec3 vec3_cross(Vec3 a, Vec3 b) {
  return vec3(a.x * b.z - a.z * b.y,
              a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

MD_INLINE Vec3 vec3_normalize(Vec3 a) {
  float len = vec3_length(a);
  return len > 0.0f ? vec3_scale(a, 1.0f / len) : vec3(0.0f, 0.0f, 0.0f);
}

MD_INLINE Vec3 vec3_lerp(Vec3 a, Vec3 b, float t) {
  return vec3(md_lerpf(a.x, b.x, t), md_lerpf(a.y, b.y, t), md_lerpf(a.z, b.z, t));
}

// Vec4

MD_INLINE Vec4 vec4(float x, float y, float z, float w) { Vec4 r = {x,y,z,w}; return r; }
MD_INLINE Vec4 vec4_add(Vec4 a, Vec4 b) { return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
MD_INLINE Vec4 vec4_sub(Vec4 a, Vec4 b) { return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
MD_INLINE Vec4 vec4_scale(Vec4 a, float s) { return vec4(a.x * s, a.y * s, a.z * s, a.w * s); }
MD_INLINE float vec4_dot(Vec4 a, Vec4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

// Mat4
Mat4 mat4_identity(void);
Mat4 mat4_mul(Mat4 a, Mat4 b);
Mat4 mat4_transpose(Mat4 m);
Vec4 mat4_mul_vec4(Mat4 m, Vec4 v);

Mat4 mat4_translate(Vec3 translation);
Mat4 mat4_scale(Vec3 s);
Mat4 mat4_rotate_x(float angle_radians);
Mat4 mat4_rotate_y(float angle_radians);
Mat4 mat4_rotate_z(float angle_radians);

Mat4 mat4_perspective(float fov, float aspect, float near, float far);
Mat4 mat4_ortho(float left, float right, float bottom, float top, float znear, float zfar);
Mat4 ma4_look_at(Vec3 eye, Vec3 center, Vec3 up);

#endif
