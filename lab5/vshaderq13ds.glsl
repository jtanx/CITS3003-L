#version 150

in  vec4 vPosition;
in  vec3 vColor;
out vec4 color;

uniform mat4 multipliers;

void main()
{
    gl_Position = multipliers * vPosition;
    color = vec4( vColor, 1.0 );
}
