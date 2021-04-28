#version 330 core

in vec2 texCords;
in vec3 aNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture_pyramid;
uniform vec3 viewPos;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 objectColor;

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.ambient;

    //light direction
    vec3 lightDir = normalize(lightPosition - fragPos);
    vec3 norm = normalize(aNormal);

    //diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;

    //specular
    float shinnes = 256;
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shinnes);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * lightColor * spec;

    fragColor = vec4(diffuse + ambient + specular, 1.0f)  * texture(texture_pyramid, texCords) ;
}