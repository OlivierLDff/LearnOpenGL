#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixRatio;

void main()
{
    vec4 text2Color = texture(texture2, TexCoord);
    FragColor = texture(texture1, TexCoord)*(1 - text2Color.a*mixRatio) + text2Color*mixRatio*text2Color.a;
}
