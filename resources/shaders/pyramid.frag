#version 330 core

in vec2 texCords;
in vec3 aNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture_pyramid;
uniform vec3 viewPos;

uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform float lightConst;
uniform float linearConst;
uniform float quadraticConst;

struct FlashLight{
    int flashlightFlag;
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutOff;
    float outterCutOff;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 sunLightDir;
uniform FlashLight flashLight;
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

    //flashlight
    float cosTheta = dot(normalize(fragPos - flashLight.position), normalize(flashLight.direction));
    float epsilon = flashLight.cutOff - flashLight.outterCutOff;
    float intensity = clamp((cosTheta - flashLight.outterCutOff)/epsilon, 0.0, 1.0);
    float flashDistance = length(flashLight.position - fragPos);
    float flashAttenuation = 1.0 / (lightConst + linearConst * flashDistance + quadraticConst * (flashDistance*flashDistance));
    vec3 flash = flashLight.color * flashAttenuation * intensity;

    if(cosTheta > flashLight.outterCutOff){
        fragColor = vec4(flash + diffuseSun + attenuation * (diffuse + specular) + ambient, 1.0)  * texture(texture_pyramid, texCords);
    }
    else
    {
        fragColor = vec4(diffuseSun + attenuation * (diffuse + specular) + ambient, 1.0)  * texture(texture_pyramid, texCords) ;
    }

}