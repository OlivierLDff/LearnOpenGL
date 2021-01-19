#version 330 core

in vec3 nearPoint;
in vec3 farPoint;

uniform mat4 viewMatrix;
uniform mat4 viewProjectionMatrix;
uniform float nearPlane;
uniform float farPlane;

uniform vec4 lineColor;
uniform vec4 backgroundColor;
uniform vec4 xAxisColor;
uniform vec4 yAxisColor;
uniform vec4 zAxisColor;
uniform float lineWidth;
uniform float lineScale;

out vec4 fragColor;

vec4 grid(vec3 pos, vec3 color, float width, float size)
{
    // Grid size
    vec3 tile = (pos - 0.5) / size;

    // Grid centered gradient
    vec3 level = abs(fract(tile) - 0.5);

    // Derivative (crisp line)
    vec3 tileDerivate = fwidth(tile);

    vec3 grid = clamp((level - tileDerivate * (width - 1.0)) / tileDerivate, 0.0, 1.0);

    float line = min(1, 1 - grid.x * grid.z);

    // z axis
    if(zAxisColor.a > 0 && pos.x > -0.1 && pos.x < 0.1)
        color = mix(color, zAxisColor.rgb, (1 - grid.x));

    // x axis
    if(xAxisColor.a > 0 && pos.z > -0.1 && pos.z < 0.1)
        color = mix(color, xAxisColor.rgb, (1 - grid.z));

    float fading = max(tileDerivate.x, max(tileDerivate.y, tileDerivate.z));
    float smoothedFading = 1 - smoothstep(0.1, 1, fading);

    return vec4(color, line * smoothedFading);
}

float computeDepth(vec3 point)
{
    vec4 clipPoint = viewProjectionMatrix * vec4(point.xyz, 1);
    return (clipPoint.z / clipPoint.w)/2 + 0.5;
}

void main()
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    if(t <= 0)
        discard;

    vec3 worldPos = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = computeDepth(worldPos);

    vec4 color = grid(worldPos, vec3(lineColor), lineWidth, lineScale);

    vec4 viewPoint = viewMatrix * vec4(worldPos, 1.0);

    float cameraDepth = (-viewPoint.z / farPlane);
    float smoothCameraDepth = 1.0 - smoothstep(0.5, 1, cameraDepth);

    if(color.a <= 0.1)
        discard;

    color.a *= smoothCameraDepth;

    fragColor = vec4(mix(color.rgb, backgroundColor.rgb, 1 - color.a), color.a);
}
