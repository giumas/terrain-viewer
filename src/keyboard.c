/**
 * keyboard.c
 */
#include "terrain.h"
#include "keyboard.h"

// Global variables defined in init.c
extern worldData world;

/**
 * Callback function to handle key events
 *  ESC - exit
 *
 *  A/D - translate objects along X axis
 *  Q/E - translate objects along Y axis
 *  W/S - translate objects along Z axis
 *
 *  i/I - rotate along X axis
 *  o/O - rotate along Y axis
 *  p/P - rotate along Z axis
 *
 *  f - toggle wireframe
 *
 *  g - toggle Gouraud/flat shading
 * 
 *  v/V - rotate sun along X axis
 * 
 *  1/! - increase/decrease shininess
 */
void keyboard( unsigned char key, int x, int y ) {
    GLfloat const DegreesToRadians = M_PI / 180.0;
    GLfloat step = world.cube_size * 0.01; // Amount to translate per step
    GLfloat angleStep = 5.0; // Amount to rotate per step
    GLfloat const sunAngleStep = 5.0; // Amount the sun moves in degrees per step

    switch(key) {
        case 033:
            exit( EXIT_SUCCESS );
            break;
        case 'W': 
            step /= 5.0;
        case 'w':
            world.viewer[0] += step * sin(world.theta[1] * DegreesToRadians);
            world.viewer[2] -= step * cos(world.theta[1] * DegreesToRadians);
            world.viewer[1] -= step * sin(world.theta[0] * DegreesToRadians); 
            break;
        case 'S': 
            step /= 5.0;
        case 's':
            world.viewer[0] -= step * sin(world.theta[1] * DegreesToRadians);
            world.viewer[2] += step * cos(world.theta[1] * DegreesToRadians);
            world.viewer[1] += step * sin(world.theta[0] * DegreesToRadians); 
            break;
        case 'D': 
            angleStep /= 5.0;
        case 'd': 
            world.theta[1] += angleStep;
            break;
        case 'A':
            angleStep /= 5.0;
        case 'a': 
            world.theta[1] -= angleStep;
            break;
        case 'E': 
            angleStep /= 5.0;
        case 'e':
            world.theta[0] -= angleStep;
            break;
        case 'Q': 
            angleStep /= 5.0;
        case 'q': 
            world.theta[0] += angleStep;
            break;
        case 'i':
            world.theta[0] += angleStep;
            break;
        case 'I':
            world.theta[0] -= angleStep;
            break;
        case 'o': 
            world.theta[1] += angleStep;
            break;
        case 'O':
            world.theta[1] -= angleStep;
            break;
        case 'p':
            world.theta[2] += angleStep;
            break;
        case 'P':
            world.theta[2] -= angleStep;
            break;
        case 'f':
            if(--world.wireframe_mode < 0) {
                world.wireframe_mode = 2;
            }
            break;
        case 'v':
            world.sun_theta += sunAngleStep;
            break;
        case 'V': 
            world.sun_theta -= sunAngleStep;
            break;
        case '1': 
            world.ground_material.shininess += 10.0;
            break;
        case '!':
            world.ground_material.shininess -= 10.0;
            if(world.ground_material.shininess < 1.0) {
                world.ground_material.shininess = 1.0;
            }
            break;
        case 'g': case 'G':
            world.flat_normals = !world.flat_normals;
            size_t const vertexSize = world.num_vertices * sizeof(vec4);
            size_t const normalSize = world.num_vertices * sizeof(vec3);
            size_t offset;
            if(world.flat_normals) {
                offset = vertexSize + normalSize;
            }else {
                offset = vertexSize;
            }
            glVertexAttribPointer(world.v_normal_pos, 3, GL_FLOAT, GL_FALSE,0, BUFFER_OFFSET(offset));
            break;
        default:
            return; // Don't redisplay if nothing updated
            break;
    }

    // Ask nicely for a redraw
    glutPostRedisplay();
}
