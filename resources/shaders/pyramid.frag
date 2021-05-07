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

uniform float lightConst;
uniform float linearConst;
uniform float quadraticConst;

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform vec3 sunLightDir;
uniform Light light;
uniform vec3 sunLightColor;
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
    vec3 diffuse = diff * lightColor;
    //sun direction
    vec3 sunLight = normalize(-sunLightDir);
    float diffSun = max(dot(norm, sunLight), 0.0);
    vec3 diffuseSun = diffSun * sunLightColor;

    //specular
    float shinnes = 256;
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shinnes);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * lightColor * spec;

    //attenuation
    float distance = length(lightPosition - fragPos);
    float attenuation = 1.0 / (lightConst + linearConst * distance + quadraticConst * (distance*distance));

    fragColor = vec4(diffuseSun + attenuation * (diffuse + specular) + ambient, 1.0)  * texture(texture_pyramid, texCords) ;
}