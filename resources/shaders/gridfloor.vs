#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 projectionViewInv;

out vec3 nearPoint;
out vec3 farPoint;

vec3 projectPoint(vec3 point, mat4 projectMatrix)
{
    vec4 projectedPoint = projectMatrix * vec4(point, 1);
    return projectedPoint.xyz / projectedPoint.w;
}

void main()
{
    nearPoint = projectPoint(vec3(aPos, 0), projectionViewInv);
    farPoint = projectPoint(vec3(aPos, 1), projectionViewInv);

    gl_Position = vec4(aPos.xy, 0, 1);
}
