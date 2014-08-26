#version 150

in  vec3 vPosition;
in  vec3 vColor;
out vec4 color;

uniform mat3 multipliers;

void main()
{
    gl_Position = vec4(multipliers * vPosition, 1.0);
    color = vec4( vColor, 1.0 );
}
