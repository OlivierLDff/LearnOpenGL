#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 ourPosition; // output a color to the fragment shader

uniform float horizontalOffset;
uniform float verticalOffset;

void main()
{
    gl_Position = vec4(aPos.x + horizontalOffset, aPos.y + verticalOffset, aPos.z, 1.0);
    ourPosition = gl_Position.xyz;
}
