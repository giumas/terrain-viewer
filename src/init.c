/**
 * init.c
 */
#include <assert.h>
#include <stdio.h>
#include "init.h"
#include "terrain.h"
#include "shader.h"

worldData world;

void
init_world_data(worldData * const w) {
    // Size of the (x,z) plane to fit the map in
    w->cube_size = 100.0;

    // The location of the camera in w coordiantes
    w->viewer[0] = 0.0;
    w->viewer[1] = w->cube_size / 2.5;
    w->viewer[2] = -1 * w->cube_size;

    // The rotation of the camera in degrees
    w->theta[0] = 15.0;
    w->theta[1] = 180.0;
    w->theta[2] = 0.0;

    // Wireframe shader parameter
    w->wireframe_mode = 0;

    // Location and properties of light representing the sun
    w->sun_theta = 0;
    vec4_init(&w->sun_light.position,0.0,w->cube_size,0.0,1.0);
    vec4_init(&w->sun_light.ambient,1.0,1.0,1.0,1.0);
    vec4_init(&w->sun_light.diffuse,1.0,1.0,1.0,1.0);
    vec4_init(&w->sun_light.specular,1.0,1.0,1.0,1.0);
    
    // Light properties of the terrain
    vec4_init(&w->ground_material.ambient,0.1,0.1,0.2,1.0);
    vec4_init(&w->ground_material.diffuse,1.0,1.0,1.0,1.0);
    vec4_init(&w->ground_material.specular,0.2,0.2,0.2,1.0);
    w->ground_material.shininess = 30.0;

    // Start with flat normal shading
    w->flat_normals = 1;
}

/**
 *  Load and store map data from a file
 *  @param[out] mData  The map data read from the file
 *  @param[in] fileData  The file to read from
 */
void 
load_file(mapData * const mData, FILE * const fileData) {
    GLfloat maxElevation = 0.0;
    GLfloat minElevation = 0.0;

    fscanf(fileData,"%u",&mData->mapWidth);
    fscanf(fileData,"%u",&mData->mapHeight);

    mData->elevationData = (GLfloat**) malloc(mData->mapHeight * sizeof(GLfloat));
    unsigned int row, col;
    for(row = 0; row < mData->mapHeight; row++) {
        mData->elevationData[row] = (GLfloat*) malloc(mData->mapWidth * sizeof(GLfloat));
        for(col = 0; col < mData->mapWidth; col++) {
            GLfloat input;
            fscanf(fileData,"%f",&input); 
            if(input < 0) {
                input = 0.0;
            }
            if(row == 0 && col == 0) {
                minElevation = input;
            }

            mData->elevationData[row][col] = input;
            if(input > maxElevation) {
                maxElevation = input;
            }
            if(input < minElevation
                    || (input > minElevation && minElevation < 1.0E-6)) {
                minElevation = input;
            }
        }
    }
    fclose(fileData);

    mData->maxElevation = maxElevation;
    mData->minElevation = minElevation;

    GLfloat const fx = (GLfloat) mData->mapWidth;
    GLfloat const fz = (GLfloat) mData->mapHeight;

    if(mData->mapWidth > mData->mapHeight) {
        mData->scale = world.cube_size / fx;
        mData->xOffset = (mData->scale * fx) / 2.0;
        mData->zOffset = (mData->scale * fz) / 2.0;
    }else {
        mData->scale = world.cube_size / fz;
        mData->xOffset = (mData->scale * fx) / 2.0;
        mData->zOffset = (mData->scale * fz) / 2.0;
    }
    mData->yScale = world.cube_size / 5.0;
    //mData->yScale = 5.0;
}

/**
 *  Convert a point from the (x,z) index to the a 4-dimensional point in
 *  world coordinates
 *  @param[out] v  The 4 dimensional point in world coordinates
 *  @param[in] x  The x index of the point in the map
 *  @param[in] z  The z index of the point in the map
 *  @param[in] mData  The current map
 */
void 
make_vertex(vec4 * const v, int x, int z, mapData const * const mData) {
    v->x = mData->scale * x - mData->xOffset;
    v->y = (mData->yScale * mData->elevationData[z][x]/mData->maxElevation) 
                    - (mData->yScale * mData->minElevation/mData->maxElevation);
    v->z = mData->scale * z - mData->zOffset;
    v->w = 1.0;
}

/**
 *  Create the flat normal for the triangular face on the bottom of a map
 *  square (a)
 *     (x, z) +--+ (x+1, z)
 *            |\a|  
 *            |b\|
 *   (x, z+1) +--+ (x+1, z+1)
 *  @param[out] n  The flat normal
 *  @param[in] x  The x index of the point in the map
 *  @param[in] z  The z index of the point in the map
 *  @param[in] mData  The current map
 */
void 
make_normal_top(vec3 * const n, int x, int z, mapData const * const mData) {
    vec4 vertex1;
    make_vertex(&vertex1, x+1, z+1, mData);
    vec4 vertex2;
    make_vertex(&vertex2, x, z, mData);
    vec4 u;
    vec4_sub(&u, &vertex1, &vertex2);

    vec4 v; 
    make_vertex(&vertex1, x+1, z, mData);
    make_vertex(&vertex2, x+1, z+1, mData);
    vec4_sub(&v,&vertex1,&vertex2);

    vec3 c;
    vec4_cross(&c,&u,&v);
    vec3_norm(n, &c);
}

/**
 *  Create the flat normal for the triangular face on the bottom of a map
 *  square (b)
 *     (x, z) +--+ (x+1, z)
 *            |\a|  
 *            |b\|
 *   (x, z+1) +--+ (x+1, z+1)
 *  @param[out] n  The flat normal
 *  @param[in] x  The x index of the point in the map
 *  @param[in] z  The z index of the point in the map
 *  @param[in] mData  The current map
 */
void 
make_normal_bottom(vec3 * const n, int x, int z, mapData const * const mData) {
    vec4 u;
    vec4 vertex1;
    vec4 vertex2;
    make_vertex(&vertex1, x, z+1, mData);
    make_vertex(&vertex2, x, z, mData);
    vec4_sub(&u,&vertex1, &vertex2);

    vec4 v; 
    make_vertex(&vertex1, x+1, z+1, mData);
    make_vertex(&vertex2, x, z+1, mData);
    vec4_sub(&v,&vertex1,&vertex2);

    vec3 c;
    vec4_cross(&c,&u,&v);
    vec3_norm(n,&c);
}

/**
 *  Return the normal for a given point by averaging the flat normals of 
 *  the faces surrounding it.
 *  @param[out] v  The averaged normal
 *  @param[in] x  The x index of the point in the map
 *  @param[in] z  The z index of the point in the map
 *  @param[in] mData  The current map 
 */
void
get_average_normal(vec3 * const v, unsigned int x, unsigned int z, mapData const * const mData) {
    vec3 n1 = {0.0,0.0,0.0};
    vec3 n2 = {0.0,0.0,0.0};
    vec3 n3 = {0.0,0.0,0.0};
    vec3 n4 = {0.0,0.0,0.0};
    if(x > 0){
        if(z < mData->mapHeight - 1) {
            make_normal_top(&n1,x-1,z,mData);
        }
        if(z > 0 && z < mData->mapHeight - 1) {
            make_normal_top(&n2,x-1,z,mData);
        }
    }
    if(x < mData->mapWidth - 2){
        if(z < mData->mapHeight - 1) {
            make_normal_top(&n3,x,z,mData);
        }
        if(z > 0 && z < mData->mapHeight) {
            make_normal_bottom(&n4,x,z-1,mData);
        }
    }

    vec3 temp;
    vec3_add(&temp,&n3,&n4);
    vec3_add(v,&n2,&temp);
    vec3_add(&temp,&n1,v);

    vec3_norm(v,&temp);
}

/**
 *  Initialize the display state using elevation data from a FILE
 *  @param[in] file  The file to load the elevation data from. 
 */
void
init(FILE* const file) {
    init_world_data(&world);

    mapData mData;
    load_file(&mData, file);

    world.num_vertices = (mData.mapHeight - 1) * (mData.mapWidth - 1) * 6;

    vec4* const vertices = (vec4*) malloc(world.num_vertices * sizeof(vec4));
    vec3* const normals = (vec3*) malloc(world.num_vertices * sizeof(vec3));
    vec3* const flat_normals = (vec3*) malloc(world.num_vertices * sizeof(vec3));

    // Calculate position of each vertex and the associated normal
    int v_index = 0;
    unsigned int z, x;
    for(z = 0; z < mData.mapHeight - 1; z++) {
        // Discrete triangles
        for(x = 0; x < mData.mapWidth - 1; x++) {
            vec4 vertex1;
            vec4 vertex2;
            make_vertex(&vertex1, x+1, z+1, &mData);
            make_vertex(&vertex2, x, z, &mData);
            vec4 u;
            vec4_sub(&u,&vertex1,&vertex2);

            vec4 v;
            make_vertex(&vertex1, x+1,z,&mData);
            make_vertex(&vertex2, x+1,z+1,&mData);
            vec4_sub(&v, &vertex1, &vertex2);
            
            vec3 n;
            vec3 c;
            vec4_cross(&c,&u,&v);
            vec3_norm( &n, &c );
                
            make_vertex(&vertices[v_index],x,z,&mData);
            get_average_normal(&normals[v_index],x,z,&mData);
            flat_normals[v_index] = n;
            v_index++;
            make_vertex(&vertices[v_index],x+1,z+1,&mData);
            get_average_normal(&normals[v_index],x+1,z+1,&mData);
            flat_normals[v_index] = n;
            v_index++;
            make_vertex(&vertices[v_index],x+1,z,&mData);
            get_average_normal(&normals[v_index],x+1,z,&mData);
            flat_normals[v_index] = n;
            v_index++;
                
            make_vertex(&vertex1,x,z+1,&mData);
            make_vertex(&vertex2,x,z,&mData);
            vec4_sub(&u, &vertex1, &vertex2);

            make_vertex(&vertex1,x+1,z+1,&mData);
            make_vertex(&vertex2,x,z+1,&mData);
            vec4_sub(&v, &vertex1, &vertex2);

            vec4_cross(&c,&u,&v);
            vec3_norm(&n,&c);

            make_vertex(&vertices[v_index],x,z,&mData);
            get_average_normal(&normals[v_index],x,z,&mData);
            flat_normals[v_index] = n;
            v_index++;
            make_vertex(&vertices[v_index],x,z+1,&mData);
            get_average_normal(&normals[v_index],x,z+1,&mData);
            flat_normals[v_index] = n;
            v_index++;
            make_vertex(&vertices[v_index],x+1,z+1,&mData);
            get_average_normal(&normals[v_index],x+1,z+1,&mData);
            flat_normals[v_index] = n;
            v_index++;

        }
    }

    // Create a vertex array object
    GLuint vao[1];
    glGenVertexArrays( 1, &vao[0] );
    glBindVertexArray( vao[0] );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // Allocate buffer
    size_t vertexSize = world.num_vertices * sizeof(vec4);
    size_t normalSize = world.num_vertices * sizeof(vec3);
    glBufferData( GL_ARRAY_BUFFER, vertexSize + normalSize + normalSize, NULL, GL_STATIC_DRAW );

        // Store the vertices as sub buffers
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertices);
        glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, normals);
        glBufferSubData(GL_ARRAY_BUFFER, vertexSize+normalSize, normalSize, flat_normals);

    // Load the shaders
    GLuint const program = init_shader("shaders/vshader.glsl", "shaders/fshader.glsl" );
    glUseProgram( program );
    
    // Initialize the vertex position attribute from the vertex shader
    GLuint const vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    // Initialize the normal position attribute from the vertex shader
    world.v_normal_pos = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( world.v_normal_pos );
    glVertexAttribPointer(world.v_normal_pos, 3, GL_FLOAT, GL_FALSE,0, BUFFER_OFFSET(vertexSize+normalSize));

    GLuint const vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize+normalSize+normalSize));

    // Calculate products for lighting and send them to shader
    vec4 ambient_product;
    vec4_mult(&ambient_product, &world.sun_light.ambient, &world.ground_material.ambient);

    vec4 diffuse_porduct;
    vec4_mult(&diffuse_porduct, &world.sun_light.diffuse, &world.ground_material.diffuse);

    vec4 specular_product;
    vec4_mult(&specular_product, &world.sun_light.specular, &world.ground_material.specular);

    glUniform4fv(glGetUniformLocation(program, "ambient_product"), 1, (GLfloat*) &ambient_product);
    glUniform4fv(glGetUniformLocation(program, "diffuse_product"), 1, (GLfloat*) &diffuse_porduct);
    glUniform4fv(glGetUniformLocation(program, "specular_product"), 1, (GLfloat*) &specular_product);

    glUniform4fv(glGetUniformLocation(program, "light_position"), 1, (GLfloat*) &world.sun_light.position);

    world.shininess_pos = glGetUniformLocation(program, "shininess");
    glUniform1f(world.shininess_pos, world.ground_material.shininess);
            
    // Get the address of the uniform cmt used for translating
    // and rotating the object, then set the defaults
    world.model_view_pos = glGetUniformLocation(program, "model_view");
    world.projection_pos = glGetUniformLocation(program, "projection");
    world.wireframe_pos = glGetUniformLocation(program, "wireframe");
    world.light_pos = glGetUniformLocation(program, "light_position");

    // Set a white background at the start
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glutSwapBuffers();

    free(normals);
    free(vertices);
    unsigned int i;
    for(i=0; i<mData.mapHeight; i++) {
        free(mData.elevationData[i]);
    }
    free(mData.elevationData);
}
