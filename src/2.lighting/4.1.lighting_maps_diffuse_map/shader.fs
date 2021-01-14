#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    vec3 diffuseColor = vec3(texture(material.diffuse, TexCoord));
    // ambient
    vec3 ambient = light.ambient * diffuseColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * diffuseColor);

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * diffuseColor);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
