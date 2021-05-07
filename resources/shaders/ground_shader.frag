#version 330 core

out vec4 fragColor;
in vec2 texCords;
uniform sampler2D texture_sand;
uniform vec3 lightColor;
uniform vec3 lightPosition;
//uniform sampler2D t1;

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 sunLightColor;
in vec3 fragPos;
uniform vec3 sunLightDir;
void main()
{
    vec3 norm = vec3(0, 1, 0);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //sun light
    vec3 sunLight = normalize(-sunLightDir);
    float diffSun = max(dot(norm, sunLight), 0.0);
    vec3 diffuseSun = diffSun * sunLightColor;


    float ambientStrength = 0.1;
    vec3 ambient = lightColor * ambientStrength;

    fragColor = vec4(diffuseSun + diffuse + ambient , 1.0f)  * texture(texture_sand, texCords);
}