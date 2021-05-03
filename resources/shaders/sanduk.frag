#version 330 core

in vec2 texCords;
in vec3 aNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;


struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform Material material;
void main()
{
    vec3 norm = normalize(aNormal);
    vec3 lightDir = normalize(lightPosition - fragPos);

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse *diff * lightColor * vec3(texture(material.diffuse, texCords));

    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - fragPos);
    float shininess = 256;
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 256);
    float specularStrenght = 0.9;
    vec3 specular = lightColor * spec * specularStrenght *  vec3(texture(material.specular, texCords));

    float ambientStrength = 0.3;
    fragColor = vec4(diffuse  +  specular, 1.0f);
}