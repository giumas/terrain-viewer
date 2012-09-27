#ifndef TERRAIN_H
#define TERRAIN_H

#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/glut.h>
// # include <GL/glut_ext.h>
#endif  // __APPLE__

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#include "vec.h"

typedef struct {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
} lightData;

typedef struct {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    GLfloat shininess;
} materialData;

typedef struct {
    GLuint mapHeight;
    GLuint mapWidth;
    GLfloat** elevationData;
    GLfloat minElevation;
    GLfloat maxElevation;
    GLfloat scale;
    GLfloat yScale;
    GLfloat xOffset;
    GLfloat zOffset;
} mapData;

typedef struct {
    GLfloat cube_size;
    GLfloat viewer[3];
    GLfloat theta[3];
    GLuint model_view_pos;
    GLuint projection_pos;
    int wireframe_mode;
    int fill_mode;
    GLuint wireframe_pos;
    lightData sun_light;
    GLfloat sun_theta;
    GLuint light_pos;
    materialData ground_material;
    GLuint shininess_pos;
    GLuint num_vertices;
} worldData;

#endif
