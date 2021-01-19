#version 330 core

in vec2 vertexPosition;

uniform mat4 inverseViewProjectionMatrix;

out vec3 nearPoint;
out vec3 farPoint;
out vec3 fragPos;

vec3 projectPoint(vec3 point, mat4 projectMatrix)
{
    vec4 projectedPoint = projectMatrix * vec4(point, 1);
    return projectedPoint.xyz / projectedPoint.w;
}

void main()
{
    // Compute nearPoint and farPoint in view space
    nearPoint = projectPoint(vec3(vertexPosition, 0), inverseViewProjectionMatrix);
    farPoint = projectPoint(vec3(vertexPosition, 1), inverseViewProjectionMatrix);

    // This geometry must fill the whole viewport
    gl_Position = vec4(vertexPosition, 0, 1);
}
