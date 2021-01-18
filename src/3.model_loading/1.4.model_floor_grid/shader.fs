#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 cameraPos;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

uniform Material material;

struct DirectionLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionLight directionLight;

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotLight;

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform PointLight pointLight;

vec3 computeDirectionLight(DirectionLight light, vec3 diffuseColor, vec3 specularColor, float shininess, vec3 normal, vec3 fragPos, vec3 cameraPos)
{
    // ambient
    vec3 ambient = light.ambient * diffuseColor;

    vec3 lightDir = normalize(-light.direction);

    // diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * diffuseColor);

    // specular
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specularColor);

    return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 diffuseColor, vec3 specularColor, float shininess, vec3 normal, vec3 fragPos, vec3 cameraPos)
{
    // ambient
    vec3 ambient = light.ambient * diffuseColor;

    vec3 lightDir = normalize(light.position - fragPos);
    float lightDistance = length(light.position - fragPos);

    float attenuation = 1.0 / (light.constant + lightDistance * light.linear + lightDistance * light.quadratic);

    // diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * diffuseColor);

    // specular
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specularColor);

    // Mix
    return (ambient + diffuse + specular) * attenuation;
}

float cutOffIntensity(float theta, float innerCutOff, float outerCutOff)
{
    return clamp((theta - outerCutOff) / (innerCutOff - outerCutOff), 0.0, 1.0);
}

vec3 computeSpotLight(SpotLight light, vec3 diffuseColor, vec3 specularColor, float shininess, vec3 normal, vec3 fragPos, vec3 cameraPos)
{
    // ambient
    vec3 ambient = light.ambient * diffuseColor;

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 spotDir = normalize(light.direction);
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = cutOffIntensity(theta, light.cutOff, light.outerCutOff);
    float lightDistance = length(light.position - fragPos);

    float attenuation = 1.0 / (light.constant + lightDistance * light.linear + lightDistance * light.quadratic);

    if(intensity > 0)
    {
        // diffuse
        vec3 norm = normalize(normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * diffuseColor);

        // specular
        vec3 viewDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = light.specular * (spec * specularColor);

        vec3 result = (ambient + (diffuse + specular) * intensity) * attenuation;
        return result;
    }
    else
    {
        return ambient * attenuation;
    }
}

void main()
{
    vec4 diffuseColor = texture(material.texture_diffuse1, TexCoord);
    vec3 specularColor = vec3(texture(material.texture_specular1, TexCoord));

    vec3 result = vec3(0);

    result += computeDirectionLight(directionLight, vec3(diffuseColor), specularColor, material.shininess, Normal, FragPos, cameraPos);

    //for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    //    result += computePointLight(pointLights[i], vec3(diffuseColor), specularColor, material.shininess, Normal, FragPos, cameraPos);

    result += computePointLight(pointLight, vec3(diffuseColor), specularColor, material.shininess, Normal, FragPos, cameraPos);

    //result += computeSpotLight(spotLight, vec3(diffuseColor), specularColor, material.shininess, Normal, FragPos, cameraPos);

    FragColor = vec4(result, diffuseColor.a);
}
