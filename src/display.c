/**
 * display.c
 */
#include <stdio.h>
#include "terrain.h"
#include "mat.h"
#include "vec.h"
#include "display.h"

// Global variables defined in init.cpp
extern GLfloat CUBE_SIZE;

extern GLfloat viewer[3];
extern GLfloat theta[3];
extern GLuint model_view_pos;

extern GLuint projection_pos;

extern int wireframe_mode;
extern GLuint wireframe_pos;

extern GLfloat sun_theta;
extern lightData sunLight;
extern GLuint light_pos;

extern materialData groundMaterial;
extern GLuint shininess_pos;

extern size_t NumVertices;

void get_model_view(mat4 r) {
    mat4 ROTATE_Z;
    mat4_rotate_z(ROTATE_Z,theta[2]);

    mat4 ROTATE_Y;
    mat4_rotate_y(ROTATE_Y,theta[1]);

    mat4 ROTATE_X;
    mat4_rotate_x(ROTATE_X,theta[0]);

    mat4 result1;
    mat4 result2;

    mat4_translate(result1,-viewer[0],-viewer[1],-viewer[2]);
    mat4_mult(result2,ROTATE_Z,result1);
    mat4_mult(result1,ROTATE_Y,result2);
    mat4_mult(r,ROTATE_X,result1);
}

void get_sun_position(vec4* r, mat4 mv) {
    vec4 temp;
    mat4 ROTATE_SUN;
    mat4_rotate_x(ROTATE_SUN,sun_theta);

    mat4_mult_v(&temp,ROTATE_SUN,&sunLight.position);
    mat4_mult_v(r,mv,&temp);
}

/**
 * Call back function called by OpenGL when a frame
 * needs to be drawn
 */
void 
display()
{
    // Clear the window
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Update model view based on camera location/rotation

    mat4 mv;
    get_model_view(mv);
    glUniformMatrix4fv(model_view_pos,1,GL_TRUE,(GLfloat*) mv);

    // Update sun position using rotation angle and translate into eye coordinates
    //vec4 const sp = mv * RotateX(sun_theta) * sunLight.position;
    vec4 sp;
    get_sun_position(&sp,mv);
    glUniform4fv(light_pos,1,(GLfloat*) &sp);

    glUniform1f(shininess_pos,groundMaterial.shininess);

    // Draw landscape
    if(wireframe_mode < 2) {
        glUniform1f(wireframe_pos,0.0);
    
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0,1.0);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glDrawArrays(GL_TRIANGLES,0,NumVertices);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    // Draw wireframe
    if(wireframe_mode > 0) {
        glUniform1f(wireframe_pos,1.0);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDrawArrays(GL_TRIANGLES,0,NumVertices);
    }

    // Update background (sky) based on light (sun) position
    vec4 night_color = {0.0,0.0,0.05,1.0};
    vec4 day_color = {0.6,0.85,1.0,1.0};

    GLfloat const sun_angle = sun_theta * M_PI / 180.0;
    GLfloat const day_percentage = (cos(sun_angle) + 1.0) / 2.0;
    GLfloat const night_percentage = 1.0 - day_percentage;

    vec4_mult_s(&night_color, &night_color, night_percentage);
    vec4_mult_s(&day_color, &day_color, day_percentage);

    vec4 sky_color;
    vec4_add(&sky_color, &night_color, &day_color);
    glClearColor( sky_color.x, sky_color.y, sky_color.z, 1.0 );

    // Double buffer
    glutSwapBuffers();
    glFlush();
}

void
reshape(int width, int height) {
    mat4 p;
    GLfloat const w = width;
    GLfloat const aspect = w / height;

    glViewport(0,0,width,height);
    mat4_perspective(p, 45.0, aspect, 0.01, CUBE_SIZE * 2.0);
    
    glUniformMatrix4fv(projection_pos, 1, GL_TRUE, (GLfloat*) p); 
}
