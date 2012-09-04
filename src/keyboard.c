/**
 * keyboard.c
 */
#include "terrain.h"
#include "keyboard.h"

// Global variables defined in init.c

extern GLfloat CUBE_SIZE;

extern GLfloat viewer[3];
extern GLfloat theta[3];

extern int wireframe_mode;

extern GLfloat sun_theta;

extern materialData groundMaterial;

extern GLuint vNormal;
extern size_t normalSize;
extern size_t vertexSize;
int flatNormal = 1;

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
 *	g - toggle Gouraud/flat shading
 * 
 *  v/V - rotate sun along X axis
 * 
 *  1/! - increase/decrease shininess
 */
void keyboard( unsigned char key, int x, int y ) {
    GLfloat const DegreesToRadians = M_PI / 180.0;
    GLfloat step = CUBE_SIZE * 0.01; // Amount to translate per step
    GLfloat angleStep = 5.0; // Amount to rotate per step
	GLfloat const sunAngleStep = 5.0; // Amount the sun moves in degrees per step
    size_t offset;

    switch(key) {
        case 033:
            exit( EXIT_SUCCESS );
            break;
        case 'W': 
			step /= 5.0;
		case 'w':
			viewer[0] += step * sin(theta[1] * DegreesToRadians);
			viewer[2] -= step * cos(theta[1] * DegreesToRadians);
			viewer[1] -= step * sin(theta[0] * DegreesToRadians); 
            break;
        case 'S': 
			step /= 5.0;
		case 's':
			viewer[0] -= step * sin(theta[1] * DegreesToRadians);
			viewer[2] += step * cos(theta[1] * DegreesToRadians);
			viewer[1] += step * sin(theta[0] * DegreesToRadians); 
            break;
        case 'D': 
			angleStep /= 5.0;
		case 'd': 
			theta[1] += angleStep;
            break;
        case 'A':
			angleStep /= 5.0;
		case 'a': 
			theta[1] -= angleStep;
            break;
        case 'E': 
			angleStep /= 5.0;
		case 'e':
			theta[0] -= angleStep;
            break;
        case 'Q': 
			angleStep /= 5.0;
		case 'q': 
			theta[0] += angleStep;
            break;
        case 'i':
            theta[0] += angleStep;
            break;
        case 'I':
            theta[0] -= angleStep;
            break;
        case 'o': 
            theta[1] += angleStep;
            break;
        case 'O':
            theta[1] -= angleStep;
            break;
        case 'p':
            theta[2] += angleStep;
            break;
        case 'P':
            theta[2] -= angleStep;
            break;
		case 'f':
			if(--wireframe_mode < 0) {
				wireframe_mode = 2;
			}
			break;
		case 'v':
			sun_theta += sunAngleStep;
			break;
		case 'V': 
			sun_theta -= sunAngleStep;
			break;
		case '1': 
			groundMaterial.shininess += 10.0;
			break;
		case '!':
			groundMaterial.shininess -= 10.0;
			if(groundMaterial.shininess < 1.0) {
				groundMaterial.shininess = 1.0;
			}
			break;
		case 'g': case 'G':
			flatNormal = !flatNormal;
			if(flatNormal) {
				offset = vertexSize + normalSize;
			}else {
				offset = vertexSize;
			}
			glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE,0, BUFFER_OFFSET(offset));
			break;
        default:
            return; // Don't redisplay if nothing updated
            break;
    }

    // Ask nicely for a redraw
    glutPostRedisplay();
}
