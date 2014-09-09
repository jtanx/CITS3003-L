/* sierpinski gasket with vertex arrays */

#include "Angel.h"
#include <ctime>

using namespace std;

// Perspective projection
mat4 projection = Frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 2.0);

// Move camera backwards relative to everything else
mat4 view = Translate(0.0, 0.0, -0.35);

const int NumTriangles = 12;  // 3^5 triangles generated
const int NumVertices = NumTriangles * 3;
vec4 points[NumVertices] = {
   vec4( -0.5, -0.5,  0.5,  1.0 ), vec4(  0.5, -0.5,  0.5,  1.0 ), vec4( -0.5,  0.5, 0.5,  1.0 ),
   vec4(  0.5,  0.5,  0.5,  1.0 ), vec4( -0.5,  0.5,  0.5,  1.0 ), vec4(  0.5, -0.5, 0.5,  1.0 ),

   vec4( -0.5, -0.5, -0.5,  1.0 ), vec4(  0.5, -0.5, -0.5,  1.0 ), vec4( -0.5,  0.5, -0.5,  1.0 ),
   vec4(  0.5,  0.5, -0.5,  1.0 ), vec4( -0.5,  0.5, -0.5,  1.0 ), vec4(  0.5, -0.5, -0.5,  1.0 ),

   vec4(  0.5, -0.5, -0.5,  1.0 ), vec4(  0.5,  0.5, -0.5,  1.0 ), vec4(  0.5, -0.5,  0.5,  1.0 ),
   vec4(  0.5,  0.5,  0.5,  1.0 ), vec4(  0.5, -0.5,  0.5,  1.0 ), vec4(  0.5,  0.5, -0.5,  1.0 ),

   vec4( -0.5, -0.5, -0.5,  1.0 ), vec4( -0.5,  0.5, -0.5,  1.0 ), vec4( -0.5, -0.5,  0.5,  1.0 ),
   vec4( -0.5,  0.5,  0.5,  1.0 ), vec4( -0.5, -0.5,  0.5,  1.0 ), vec4( -0.5,  0.5, -0.5,  1.0 ),

   vec4( -0.5,  0.5, -0.5,  1.0 ), vec4( -0.5,  0.5,  0.5,  1.0 ), vec4(  0.5,  0.5, -0.5,  1.0 ),
   vec4(  0.5,  0.5,  0.5,  1.0 ), vec4(  0.5,  0.5, -0.5,  1.0 ), vec4( -0.5,  0.5,  0.5,  1.0 ),

   vec4( -0.5, -0.5, -0.5,  1.0 ), vec4( -0.5, -0.5,  0.5,  1.0 ), vec4(  0.5, -0.5, -0.5,  1.0 ),
   vec4(  0.5, -0.5,  0.5,  1.0 ), vec4(  0.5, -0.5, -0.5,  1.0 ), vec4( -0.5, -0.5,  0.5,  1.0 ),
};

vec3 colours[NumVertices] = {
	vec3(0.9,0.9,0.9), vec3(0.1,0.40,0.7), vec3(0.5,0.2,0.3),
	vec3(0.9,0.9,0.9), vec3(0.5,0.2,0.3), vec3(0.1,0.40,0.7),
	
	vec3(0.9,0.9,0.9), vec3(0.1,0.40,0.7), vec3(0.5,0.2,0.3),
	vec3(0.9,0.9,0.9), vec3(0.5,0.2,0.3), vec3(0.1,0.40,0.7),
	
	vec3(0.9,0.9,0.9), vec3(0.1,0.40,0.7), vec3(0.5,0.2,0.3),
	vec3(0.9,0.9,0.9), vec3(0.5,0.2,0.3), vec3(0.1,0.40,0.7),
	
	vec3(0.9,0.9,0.9), vec3(0.1,0.40,0.7), vec3(0.5,0.2,0.3),
	vec3(0.9,0.9,0.9), vec3(0.5,0.2,0.3), vec3(0.1,0.40,0.7),
	
	vec3(0.9,0.9,0.9), vec3(0.1,0.40,0.7), vec3(0.5,0.2,0.3),
	vec3(0.9,0.9,0.9), vec3(0.5,0.2,0.3), vec3(0.1,0.40,0.7),
	
	vec3(0.9,0.9,0.9), vec3(0.1,0.40,0.7), vec3(0.5,0.2,0.3),
	vec3(0.9,0.9,0.9), vec3(0.5,0.2,0.3), vec3(0.1,0.40,0.7)
	
};
int Index = 0;

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

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshaderq13ds.glsl", "fshader24.glsl" );
    glUseProgram( program );

    multipliers = glGetUniformLocation(program, "multipliers");

    // Initialize the vertex position attribute from the vertex shader    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
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
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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

    glutMainLoop();
    return 0;
}
