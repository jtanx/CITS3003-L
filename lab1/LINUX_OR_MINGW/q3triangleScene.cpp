/* sierpinski gasket with vertex arrays */

#include "Angel.h"
#include <ctime>

using namespace std;

const int NumTriangles = 3;  // 3^5 triangles generated
const int NumVertices = NumTriangles * 3;
vec2 points[NumVertices];
int Index = 0;

//----------------------------------------------------------------------------

void
triangle( const vec2& a, const vec2& b, const vec2& c )
{
    points[Index++] = a;
    points[Index++] = b;
    points[Index++] = c;
}

float rrand() {
	return (((float)rand()) / ((float)RAND_MAX / 2)) - 1;
}

vec2 prand() {
	return vec2(rrand(), rrand());
}

void makeTriangles() {
	Index = 0;
	for (int i = 0; i < NumTriangles; i++) {
		triangle(prand(), prand(), prand());
	}
}

//----------------------------------------------------------------------------

void
init( void )
{
	srand(time(NULL));
    vec2 vertices[3] = {
        vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
    };
	
	makeTriangles();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader23.glsl", "fshader23.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader    
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

#ifdef __MINGW32__
//doesn't work
void
wheel(int wheel, int direction, int x, int y)
{
	makeTriangles();
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
}

#endif

void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
			case GLUT_LEFT_BUTTON: case 3: case 4:
				makeTriangles();
				glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
			break;
		}
	}
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Simple GLSL example" );

    glewInit();

    init();

    glutDisplayFunc( display );
	glutMouseFunc(mouse);
    glutKeyboardFunc( keyboard );
#ifdef __MINGW32__
	glutMouseWheelFunc(wheel);
#endif

    glutMainLoop();
    return 0;
}
