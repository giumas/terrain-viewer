#include <stdio.h>
#include <math.h>
#include "mat.h"

inline
void mat4_clear(mat4 r) {
    unsigned int i,j;
    for(i=0; i<4; ++i){
        for(j=0; j<4; ++j) {
            r[i][j] = 0.0;
        }
    }
}

inline
void mat4_create_i(mat4 r) {
    unsigned int i,j;
    for(i=0; i<4; ++i){
        for(j=0; j<4; ++j) {
            r[i][j] = (i==j) ? 1.0 : 0.0;
        }
    }
}

inline
void mat4_print(mat4 r) { unsigned int i,j;
    for(i=0; i<4; ++i){
        for(j=0; j<4; ++j) {
            printf("%f ",r[i][j]);
        }
        printf("\n");
    }
}

inline
void mat4_translate(mat4 r, GLfloat x, GLfloat y, GLfloat z) {
    mat4_create_i(r);
    r[0][3] = x;
    r[1][3] = y;
    r[2][3] = z;
}

inline
void mat4_rotate_x(mat4 r, GLfloat theta) {
    GLfloat const DegreesToRadians = M_PI / 180.0;
    GLfloat const angle = DegreesToRadians * theta;
    mat4_create_i(r);
    r[2][2] = r[1][1] = cos(angle);
    r[2][1] = sin(angle);
    r[1][2] = -r[2][1];
}

inline 
void mat4_rotate_y(mat4 r, GLfloat theta) {
    GLfloat const DegreesToRadians = M_PI / 180.0;
    GLfloat angle = DegreesToRadians * theta;
    mat4_create_i(r);
    r[2][2] = r[0][0] = cos(angle);
    r[0][2] = sin(angle);
    r[2][0] = -r[0][2];
}

inline 
void mat4_rotate_z(mat4 r, GLfloat theta) {
    GLfloat const DegreesToRadians = M_PI / 180.0;
    GLfloat const angle = DegreesToRadians * theta;
    mat4_create_i(r);
    r[0][0] = r[1][1] = cos(angle);
    r[1][0] = sin(angle);
    r[0][1] = -r[1][0];
}

inline
void mat4_mult(mat4 r, mat4 s, mat4 t) {
    unsigned int i,j,k;
    mat4_clear(r);
    for(i=0; i<4; ++i) {
        for(j=0; j<4; ++j) {
            for(k=0; k<4; ++k) {
                r[i][j] += s[i][k] * t[k][j];
            }
        }
    }
}

inline 
void mat4_mult_v(vec4 * const r, mat4 m, vec4 const * const v) {
    r->x = m[0][0]*v->x + m[0][1]*v->y + m[0][2]*v->z + m[0][3]*v->w;
    r->y = m[1][0]*v->x + m[1][1]*v->y + m[1][2]*v->z + m[1][3]*v->w;
    r->z = m[2][0]*v->x + m[2][1]*v->y + m[2][2]*v->z + m[2][3]*v->w;
    r->w = m[3][0]*v->x + m[3][1]*v->y + m[3][2]*v->z + m[3][3]*v->w;
}

inline
void mat4_perspective(mat4 r, GLfloat fovy, GLfloat aspect,
		  GLfloat zNear, GLfloat zFar) {
    GLfloat const DegreesToRadians = M_PI / 180.0;
    GLfloat const top   = tan(fovy*DegreesToRadians/2) * zNear;
    GLfloat const right = top * aspect;
    mat4_create_i(r);
    r[0][0] = zNear/right;
    r[1][1] = zNear/top;
    r[2][2] = -(zFar + zNear)/(zFar - zNear);
    r[2][3] = -2.0*zFar*zNear/(zFar - zNear);
    r[3][2] = -1.0;
}
