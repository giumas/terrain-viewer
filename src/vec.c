#include "vec.h"

inline 
void vec4_init(vec4 * const v, GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
    v->x = a;
    v->y = b;
    v->z = c;
    v->w = d;
}

inline
GLfloat vec2_dot(vec2 const * const u, vec2 const * const v) {
    return u->x * v->x + u->y * v->y;
}

inline 
GLfloat vec3_dot(vec3 const * const u, vec3 const * const v) {
    return u->x * v->x 
           + u->y * v->y 
           + u->z * v->z;
}

inline 
GLfloat vec4_dot(vec4 const * const u, vec4 const * const v) {
    return u->x * v->x 
           + u->y * v->y 
           + u->z * v->z
           + u->w * v->w;
}

inline 
GLfloat vec2_length(vec2 const * const v) {
    return sqrt( vec2_dot(v,v) );
}

inline
GLfloat vec3_length(vec3 const * const v) {
    return sqrt( vec3_dot(v,v) );
}

inline
GLfloat vec4_length(vec4 const * const v) {
    return sqrt( vec4_dot(v,v) );
}

inline
void vec2_mult_s(vec2 * const r, vec2 const * const v, GLfloat s) {
    r->x = v->x * s;
    r->y = v->y * s;
}

inline
void vec3_mult_s(vec3 * const r, vec3 const * const v, GLfloat s) {
    r->x = v->x * s;
    r->y = v->y * s;
    r->z = v->z * s;
}

inline
void vec4_mult_s(vec4 * const r, vec4 const * const v, GLfloat s) {
    r->x = v->x * s;
    r->y = v->y * s;
    r->z = v->z * s;
    r->w = v->w * s;
}

inline
void vec2_div_s(vec2 * const r, vec2 const * const v, GLfloat s) {
    r->x = v->x / s;
    r->y = v->y / s;
}

inline
void vec3_div_s(vec3 * const r, vec3 const * const v, GLfloat s) {
    r->x = v->x / s;
    r->y = v->y / s;
    r->z = v->z / s;
}

inline
void vec4_div_s(vec4 * const r, vec4 const * const v, GLfloat s) {
    r->x = v->x / s;
    r->y = v->y / s;
    r->z = v->z / s;
    r->w = v->w / s;
}

inline
void vec2_add(vec2 * const r, vec2 const * const u, vec2 const * const v) {
    r->x = u->x + v->x;
    r->y = u->y + v->y;
}

inline void 
vec3_add(vec3 * const r, vec3 const * const u, vec3 const * const v) {
    r->x = u->x + v->x;
    r->y = u->y + v->y;
    r->z = u->z + v->z;
}

inline
void vec4_add(vec4 * const r, vec4 const * const u, vec4 const * const v) {
    r->x = u->x + v->x;
    r->y = u->y + v->y;
    r->z = u->z + v->z;
    r->w = u->w + v->w;
}

inline
void vec2_sub(vec2 * const r, vec2 const * const u, vec2 const * const v) {
    r->x = u->x - v->x;
    r->y = u->y - v->y;
}

inline
void vec3_sub(vec3 * const r, vec3 const * const u, vec3 const * const v) {
    r->x = u->x - v->x;
    r->y = u->y - v->y;
    r->z = u->z - v->z;
}

inline
void vec4_sub(vec4 * const r, vec4 const * const u, vec4 const * const v) {
    r->x = u->x - v->x;
    r->y = u->y - v->y;
    r->z = u->z - v->z;
    r->w = u->w - v->w;
}

inline
void vec2_mult(vec2 * const r, vec2 const * const u, vec2 const * const v) {
    r->x = u->x * v->x;
    r->y = u->y * v->y;
}

inline
void vec3_mult(vec3 * const r, vec3 const * const u, vec3 const * const v) {
    r->x = u->x * v->x;
    r->y = u->y * v->y;
    r->z = u->z * v->z;
}

inline
void vec4_mult(vec4 * const r, vec4 const * const u, vec4* v) {
    r->x = u->x * v->x;
    r->y = u->y * v->y;
    r->z = u->z * v->z;
    r->w = u->w * v->w;
}

inline 
void vec3_cross(vec3 * const r, vec3 const * const u, vec3 const * const v) {
    r->x = u->y * v->z - u->z * v->y;
    r->y = u->z * v->x - u->x * v->z;
    r->z = u->x * v->y - u->y * v->x;
}

inline 
void vec4_cross(vec3 * const r, vec4 const * const u, vec4 const * const v) {
    r->x = u->y * v->z - u->z * v->y;
    r->y = u->z * v->x - u->x * v->z;
    r->z = u->x * v->y - u->y * v->x;
}

inline
void vec3_norm(vec3 * const r, vec3 const * const v) {
    vec3_div_s(r,v,vec3_length(v));
}

inline
void vec4_norm(vec3 * const r, vec4 * const v) {
    vec4 r2;
    vec4_div_s(&r2,v,vec4_length(v));
    r->x = r2.x;
    r->y = r2.y;
    r->z = r2.z;
}
