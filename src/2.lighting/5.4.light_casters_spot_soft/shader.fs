#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 cameraPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform SpotLight light;

float cutOffIntensity(float theta, float innerCutOff, float outerCutOff)
{
    return clamp((theta - outerCutOff) / (innerCutOff - outerCutOff), 0.0, 1.0);
}

void main()
{
    vec3 diffuseColor = vec3(texture(material.diffuse, TexCoord));
    vec3 specularColor = vec3(texture(material.specular, TexCoord));

    // ambient
    vec3 ambient = light.ambient * diffuseColor;

    vec3 lightDir = normalize(light.position - FragPos);
    vec3 spotDir = normalize(light.direction);
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = cutOffIntensity(theta, light.cutOff, light.outerCutOff);

    if(intensity > 0)
    {
        // diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * diffuseColor);

        // specular
        vec3 viewDir = normalize(cameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * specularColor);

        vec3 result = ambient + (diffuse + specular) * intensity;
        FragColor = vec4(result, 1.0);
    }
    else
    {
        FragColor = vec4(ambient, 1.0);
    }
}
