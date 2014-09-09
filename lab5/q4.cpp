/* sierpinski gasket with vertex arrays */

#include "Angel.h"
#include <ctime>

using namespace std;

int numDisplayCalls = 0;
char lab[] = "Lab5";
char programName[] = "q3callbacks";

// Perspective projection
mat4 projection = Frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 2.0);

// Move camera backwards relative to everything else
mat4 view = Translate(0.0, 0.0, -0.35);

const int NumTriangles = 12;
const int NumVertices  = 8;
const int NumElements  = 3 * NumTriangles;

// Each vertex now appears only once, so we have only 8 rather than 36
vec3 points[NumVertices] = {
   vec3( -0.5, -0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ),
   vec3( -0.5,  0.5, -0.5 ), vec3( -0.5,  0.5,  0.5 ),

   vec3(  0.5, -0.5, -0.5 ), vec3(  0.5, -0.5,  0.5 ),
   vec3(  0.5,  0.5, -0.5 ), vec3(  0.5,  0.5,  0.5 )
};

// The following builds triangles from the 8 vertices above,
// using numbers 0-7 to refer to the element positions in the array
GLuint elements[NumElements] = {
    1, 5, 3,
    7, 3, 5,

    0, 4, 2,
    6, 2, 4,

    4, 6, 5,
    7, 5, 6,

    0, 2, 1,
    3, 1, 2,

    2, 3, 6,
    7, 6, 3,

    0, 1, 4,
    5, 4, 1
};


// We only need to give 8 colors, one for each vertex.
vec3 colours[NumVertices] = {
  vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0),
  vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 1.0),

  vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 1.0),
  vec3(1.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0),
};

GLuint multipliers;
GLint windowWidth, windowHeight;


//----------------------------------------------------------------------------

void
init( void )
{
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // First, we create an empty buffer of the size we need by passing
    //   a NULL pointer for the data values
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colours),
		  NULL, GL_STATIC_DRAW );

    // Next, we load the real data in parts.  We need to specify the
    //   correct byte offset for placing the color data after the point
    //   data in the buffer.  Conveniently, the byte offset we need is
    //   the same as the size (in bytes) of the points array, which is
    //   returned from "sizeof(points)".
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colours), colours );
    
    GLuint elementBufferId;
    glGenBuffers(1, &elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshaderq13ds.glsl", "fshader24.glsl" );
    glUseProgram( program );

    multipliers = glGetUniformLocation(program, "multipliers");

    // Initialize the vertex position attribute from the vertex shader    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    // Likewise, initialize the vertex color attribute.  Once again, we
    //    need to specify the starting offset (in bytes) for the color
    //    data.  Just like loading the array, we use "sizeof(points)"
    //    to determine the correct value.
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(points)) );

    glEnable( GL_DEPTH_TEST );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void drawCube(mat4 model) {
    glUniformMatrix4fv(multipliers, 1, GL_TRUE, projection * view * model);
    glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_INT, NULL);
}

void
display( void )
{
    float angle = glutGet(GLUT_ELAPSED_TIME) * 0.001 * 180.0/3.141592654;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 baseTL = Translate(0.5, 0.5, 0) * Scale(0.3);
    drawCube(baseTL * RotateX(angle) * RotateY(angle));
    drawCube(baseTL * RotateX(angle) * RotateY(angle) * Translate(0.6, 0.6, 0.6));
    
    mat4 baseBL = Translate(0.5, -0.5, 0) * Scale(0.3);
    drawCube(baseBL * RotateX(angle) * RotateY(angle));
    drawCube(baseBL * RotateX(angle) * Translate(0.6, 0.6, 0.6) * RotateY(angle));
    
    mat4 baseRL = Translate(-0.5, -0.5, 0) * Scale(0.3);
    drawCube(baseRL * RotateX(angle) * RotateY(angle));
    drawCube(baseRL * Translate(0.6, 0.6, 0.6) * RotateX(angle) * RotateY(angle));
    
    drawCube(Translate(-0.5, 0.5, 0) * Scale(0.3) * RotateX(angle) * RotateY(angle));
    
    //glUniformMatrix3fv(multipliers, 1, GL_FALSE, zTransform);
    //glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    
    //glUniformMatrix3fv(multipliers, 1, GL_FALSE, xTransform);
    //glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    glFlush();
    
    numDisplayCalls++;
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

void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
			case GLUT_LEFT_BUTTON: case 3: case 4:
			break;
		}
	}
}

void
idle(void)
{
    glutPostRedisplay();
}

void shaper(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    
    float rat = (float)width/height*0.2;
    projection = Frustum(-rat, rat, -0.2, 0.2, 0.2, 2.0);
    
    glViewport(0, 0, windowWidth, windowHeight);
}

void motion(int x, int y) {
    view = Translate((float)x / windowWidth - 0.5, -(float)y / windowHeight + 0.5, -0.35);
}

void timer(int t) {
    char title[256];
    
    sprintf(title, "%s %s: %d Frames Per Second @ %d x %d",
            lab, programName, numDisplayCalls, windowWidth, windowHeight );
 
    glutSetWindowTitle(title);
    
    numDisplayCalls = 0;
    glutTimerFunc(1000, timer, 0);
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Simple GLSL example" );

    glewInit();

    init();

    glutDisplayFunc( display );
	glutMouseFunc(mouse);
    glutKeyboardFunc( keyboard );
    glutIdleFunc(idle);
    glutReshapeFunc(shaper);
    glutMotionFunc(motion);
    glutTimerFunc(1000, timer, 0);

    glutMainLoop();
    return 0;
}
