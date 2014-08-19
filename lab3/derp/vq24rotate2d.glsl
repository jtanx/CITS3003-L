#version 150

in  vec4 vPosition;
in  vec3 vColor;
out vec4 color;
uniform mat2 multipliers;

void
main()
{
    /*gl_Position = vec4(vPosition.x*multipliers[0][0] + vPosition.y*multipliers[0][1],
                       vPosition.x*multipliers[1][0] + vPosition.y*multipliers[1][1],
                       0.0,
                       1.0);
    */               
    gl_Position.xy = multipliers * vPosition.xy;
    //gl_Position = vPosition;
    color = vec4( vColor, 1.0 );
}
