#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat3 normalMatrix;
uniform mat4 modelView;
uniform mat4 projection;

void main()
{
    // Compute frag position in world position
    FragPos = vec3(modelView * vec4(aPos, 1.0));

    // Compute normal after world translation/rotation/scale
    Normal =  normalMatrix * aNormal;

    TexCoord = aTexCoord;

    gl_Position = projection * vec4(FragPos, 1.0);
}
