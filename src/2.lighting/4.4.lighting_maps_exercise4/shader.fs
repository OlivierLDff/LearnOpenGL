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

uniform sampler2D emissive;
uniform float emissiveDimmer;

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
    vec3 specularColor = vec3(texture(material.specular, TexCoord));

    // emissive
    vec3 emissiveColor = vec3(texture(emissive, TexCoord));

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
    vec3 specular = light.specular * (spec * specularColor);

    vec3 emissiveColorMask = vec3(1) - step(vec3(0.001), specularColor);

    vec3 result = (emissiveDimmer * emissiveColor * emissiveColorMask) + ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
