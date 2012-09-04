/**
 * main.c
 */
#include <stdio.h>
#include "display.h"
#include "keyboard.h"
#include "init.h"

int main(int argc, char* argv[]) {

    glutInit( &argc, argv );

    // Init with double and depth buffering
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    
    glutCreateWindow( "Terrain Viewer" );
    
    glewInit();

    FILE* elevation_file = NULL;
    if(argc > 1) {
        elevation_file = fopen(argv[1],"r");
        if(elevation_file == NULL) {
            fprintf(stderr, "Unable to open file: %s\n", argv[1]);
            exit(1);
        }
    }else {
        elevation_file = stdin;
    }


    // Initialize objects
    init(elevation_file);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
