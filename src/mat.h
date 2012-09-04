#ifndef TERRAIN_MAT_H
#define TERRAIN_MAT_H

#include "vec.h"

typedef GLfloat mat4[4][4];

inline
void mat4_clear(mat4 r);

inline
void mat4_create_i(mat4 r);

inline
void mat4_print(mat4 r);

inline
void mat4_translate(mat4 r, GLfloat x, GLfloat y, GLfloat z);

inline
void mat4_rotate_x(mat4 r, GLfloat theta);

inline 
void mat4_rotate_y(mat4 r, GLfloat theta);

inline 
void mat4_rotate_z(mat4 r, GLfloat theta);

inline
void mat4_mult(mat4 r, mat4 s, mat4 t);

inline 
void mat4_mult_v(vec4 * const r, mat4 m, vec4 const * const v);

inline
void mat4_perspective(mat4 r, GLfloat fovy, GLfloat aspect,
          GLfloat zNear, GLfloat zFar);

#endif
