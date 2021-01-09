#version 330 core
out vec4 FragColor;
in vec3 ourPosition;

void main()
{
    FragColor = vec4((ourPosition*0.5)+vec3(0.5, 0.5, 0), 1.0);
}
