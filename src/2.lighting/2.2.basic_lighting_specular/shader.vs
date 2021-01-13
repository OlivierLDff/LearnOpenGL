#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat3 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Compute frag position in world position
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Compute normal after world translation/rotation/scale
    Normal =  normalMatrix * aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
