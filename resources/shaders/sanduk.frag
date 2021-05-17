#version 330 core

in vec2 texCords;
in vec3 aNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec3 sunLightDirection;
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

struct SpotLight{
    float lightConst;
    float linearConst;
    float quadraticConst;

    int spotLightFlag;
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutOff;
    float outerCutOff;
};


uniform Light light;
uniform SpotLight spotLight;
uniform Material material;


vec3 calculateSpotLight(SpotLight spotLight, vec3 fragPos, vec3 viewPos, vec3 norm);

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

    vec3 sunLightDir = normalize(sunLightDirection - fragPos);
    float sunDiff = max(dot(-sunLightDir, norm), 0.0);
    vec3 sunDiffuse = sunDiff * vec3(texture(material.diffuse, texCords));

    vec3 spotLightVec = calculateSpotLight(spotLight, fragPos, viewPos, norm);

    fragColor = vec4(sunDiffuse + diffuse  +  specular + spotLightVec, 1.0f);
}

vec3 calculateSpotLight(SpotLight spotLight, vec3 fragPos, vec3 viewPos, vec3 norm){

    //light beam direction for each fragment
    vec3 lightDir = normalize(fragPos - spotLight.position);

    //attenuation calclutation
    float distance = length(fragPos - spotLight.position); //not using lightDir because lightDir is normalized
    float attenuation = 1.0 / (spotLight.lightConst + spotLight.linearConst * distance + spotLight.quadraticConst * (distance*distance));

    //diffuse
    float diff = max(dot(-lightDir, norm),0.0);
    vec3 diffuse = diff * spotLight.color;
    diffuse *= attenuation;

    //specular
    float shiness = 32;
    float specularStrength = 0.5;

    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(fragPos - viewPos);

    float spec = pow(max(dot(-viewDir, reflectDir), 0.0), shiness);
    vec3 specular = specularStrength * spotLight.color * spec;
    specular *= attenuation;

    //spot
    float cosTheta = dot(lightDir, normalize(spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((cosTheta - spotLight.outerCutOff)/epsilon, 0.0, 1.0);
    float spotDistance = length(spotLight.position - fragPos);
    float spotAttenuation = 1.0 / (spotLight.lightConst + spotLight.linearConst * spotDistance + spotLight.quadraticConst * (spotDistance*spotDistance));
    vec3 flash = spotLight.color * attenuation * intensity;

    vec3 spot = vec3(0.0, 0.0, 0.0);

    if(spotLight.spotLightFlag == 1 && cosTheta > spotLight.outerCutOff){
        spot = diffuse + specular + flash;
    }

    return spot * vec3(texture(material.diffuse, texCords));
}