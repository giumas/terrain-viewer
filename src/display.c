/**
 * display.c
 */
#include <stdio.h>
#include "terrain.h"
#include "mat.h"
#include "vec.h"
#include "display.h"

// Global variables defined in init.c
extern worldData world;

void get_model_view(mat4 r) {
    mat4 ROTATE_Z;
    mat4_rotate_z(ROTATE_Z, world.theta[2]);

    mat4 ROTATE_Y;
    mat4_rotate_y(ROTATE_Y, world.theta[1]);

    mat4 ROTATE_X;
    mat4_rotate_x(ROTATE_X, world.theta[0]);

    mat4 result1;
    mat4 result2;

    mat4_translate(result1, -world.viewer[0], -world.viewer[1], -world.viewer[2]);
    mat4_mult(result2, ROTATE_Z, result1);
    mat4_mult(result1, ROTATE_Y, result2);
    mat4_mult(r, ROTATE_X, result1);
}

void get_sun_position(vec4* r, mat4 mv) {
    vec4 temp;
    mat4 ROTATE_SUN;
    mat4_rotate_x(ROTATE_SUN, world.sun_theta);

    mat4_mult_v(&temp, ROTATE_SUN, &world.sun_light.position);
    mat4_mult_v(r, mv, &temp);
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
    glUniformMatrix4fv(world.model_view_pos, 1, GL_TRUE, (GLfloat*) mv);

    // Update sun position using rotation angle and translate into eye coordinates
    vec4 sp;
    get_sun_position(&sp, mv);
    glUniform4fv(world.light_pos, 1, (GLfloat*) &sp);

    glUniform1f(world.shininess_pos, world.ground_material.shininess);

    // Draw landscape
    if(world.wireframe_mode < 2) {
        glUniform1f(world.wireframe_pos, 0.0);
    
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, world.num_vertices);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    // Draw wireframe
    if(world.wireframe_mode > 0) {
        glUniform1f(world.wireframe_pos, 1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, world.num_vertices);
    }

    // Update background (sky) based on light (sun) position
    vec4 night_color = {0.0, 0.0, 0.05, 1.0};
    vec4 day_color = {0.6, 0.85, 1.0, 1.0};

    GLfloat const sun_angle = world.sun_theta * M_PI / 180.0;
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

    glViewport(0, 0, width, height);
    mat4_perspective(p, 45.0, aspect, 0.01, world.cube_size * 2.0);
    
    glUniformMatrix4fv(world.projection_pos, 1, GL_TRUE, (GLfloat*) p); 
}
