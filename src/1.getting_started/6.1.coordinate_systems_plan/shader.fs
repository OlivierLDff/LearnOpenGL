#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = vec4(1.f, 0.f, 0.5f, 1.f);
}
