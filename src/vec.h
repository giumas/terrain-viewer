#ifndef TERRAIN_VEC_H
#define TERRAIN_VEC_H

#include <math.h>
#include <GL/glut.h>

typedef struct vec2 {
    GLfloat x;
    GLfloat y;
} vec2;

typedef struct vec3 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vec3;

typedef struct vec4 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

inline 
void vec4_init(vec4 * const v, GLfloat a, GLfloat b, GLfloat c, GLfloat d);

inline
GLfloat vec2_dot(vec2 const * const u, vec2 const * const v);

inline 
GLfloat vec3_dot(vec3 const * const u, vec3 const * const v);

inline 
GLfloat vec4_dot(vec4 const * const u, vec4 const * const v);

inline 
GLfloat vec2_length(vec2 const * const v);

inline
GLfloat vec3_length(vec3 const * const v);

inline
GLfloat vec4_length(vec4 const * const v);

inline
void vec2_mult_s(vec2 * const r, vec2 const * const v, GLfloat s);

inline
void vec3_mult_s(vec3 * const r, vec3 const * const v, GLfloat s);

inline
void vec4_mult_s(vec4 * const r, vec4 const * const v, GLfloat s);

inline
void vec2_div_s(vec2 * const r, vec2 const * const v, GLfloat s);

inline
void vec3_div_s(vec3 * const r, vec3 const * const v, GLfloat s);

inline
void vec4_div_s(vec4 * const r, vec4 const * const v, GLfloat s);

inline
void vec2_add(vec2 * const r, vec2 const * const u, vec2 const * const v);

inline void 
vec3_add(vec3 * const r, vec3 const * const u, vec3 const * const v);

inline
void vec4_add(vec4 * const r, vec4 const * const u, vec4 const * const v);

inline
void vec2_sub(vec2 * const r, vec2 const * const u, vec2 const * const v);

inline
void vec3_sub(vec3 * const r, vec3 const * const u, vec3 const * const v);

inline
void vec4_sub(vec4 * const r, vec4 const * const u, vec4 const * const v);

inline
void vec2_mult(vec2 * const r, vec2 const * const u, vec2 const * const v);

inline
void vec3_mult(vec3 * const r, vec3 const * const u, vec3 const * const v);

inline
void vec4_mult(vec4 * const r, vec4 const * const u, vec4* v);

inline 
void vec3_cross(vec3 * const r, vec3 const * const u, vec3 const * const v);

inline 
void vec4_cross(vec3 * const r, vec4 const * const u, vec4 const * const v);

inline
void vec3_norm(vec3 * const r, vec3 const * const v);

inline
void vec4_norm(vec3 * const r, vec4 * const v);

#endif
