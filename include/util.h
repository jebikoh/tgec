#ifndef UTIL_H
#define UTIL_H

#include <math.h>

typedef struct {
    float x, y, z;
} Vec3;

void init_vec3(Vec3 *v, float x, float y, float z);
void add_vec3(Vec3 *v1, const Vec3 *v2);
void sub_vec3(Vec3 *v1, const Vec3 *v2);
void normalize_vec3(Vec3 *v);

typedef enum {
    X = 0,
    Y = 1, 
    Z = 2
} Axis;

typedef struct {
    float m[4][4];
} Mat4;

float deg_to_rad(float deg);
void set_identity_mat(Mat4 *m);
void build_rotation(Mat4 *m, float rad, Axis axis);
void build_translation(Mat4 *m, float x, float y, float z);
void build_scale(Mat4 *m, float x, float y, float z);

#endif // UTIL_H