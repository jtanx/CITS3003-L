#version 150

in  vec4 vPosition;
in  vec3 vColor;
out vec4 color;
uniform mat2 multipliers;

void
main()
{
    //yz rotation
    //gl_Position.x = vPosition.x;               
    //gl_Position.yz = multipliers * vPosition.yz;
    
    //xz rotation
    gl_Position.y = vPosition.y;
    gl_Position.xz = multipliers * vPosition.xz;
    //gl_Position = vPosition;
    color = vec4( vColor, 1.0 );
}
