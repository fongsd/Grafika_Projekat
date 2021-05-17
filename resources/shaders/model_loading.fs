#version 330 core

in vec2 texCords;
in vec3 aNormal;
in vec3 fragPos;

uniform sampler2D diffuse_texture1;

uniform vec3 sunLightColor;
uniform vec3 sunLightDirection;
uniform vec3 lightPosition;
out vec4 fragColor;

uniform vec3 viewPos;

struct DirLight{
    vec3 direction;
    vec3 color;
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

uniform SpotLight spotLight;
uniform DirLight dirLight;

vec3 calculateDirLight(DirLight dirLight, vec3 fragPos, vec3 viewPos, vec3 norm);
vec3 calculateSpotLight(SpotLight spotLight, vec3 fragPos, vec3 viewPos, vec3 norm);
void main()
{
    vec3 norm = normalize(aNormal);
    vec3 lightDir = normalize(lightPosition - fragPos);

//     vec3 DirLight = calculateDirLight(dirLight, fragPos, viewPos, norm);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff *  vec3(texture(diffuse_texture1, texCords));
    //sunLight
    vec3 sunLightDir = normalize(sunLightDirection - fragPos);
    float sunDiff = max(dot(-sunLightDir, norm), 0.0);
    vec3 sunDiffuse = sunDiff * vec3(texture(diffuse_texture1, texCords));

    //spotLight

    vec3 lightDirSpotLight = calculateSpotLight(spotLight, fragPos, viewPos, norm);


    //end of spotLight

    float ambientStrength = 0.3;
    fragColor = vec4(sunDiffuse + diffuse + lightDirSpotLight , 1.0f) ;
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

    return spot * vec3(texture(diffuse_texture1, texCords));
}

vec3 calculateDirLight(DirLight dirLight, vec3 fragPos, vec3 viewPos, vec3 norm){
    //light beam direction for each fragment
    vec3 lightDir = normalize(dirLight.direction);

    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * dirLight.color;

    //diffuse
    float diff = max(dot(-lightDir, norm),0.0);
    vec3 diffuse = diff * dirLight.color;

    //specular
    float shinnes = 16;
    vec3 reflectDir = reflect(lightDir, norm);
    vec3 viewDir = normalize(fragPos - viewPos);
    float spec = pow(max(dot(-viewDir, reflectDir), 0.0), shinnes);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * dirLight.color * spec;

    vec3 dir = ambient + diffuse;
    return dir * vec3(texture(diffuse_texture1, texCords));
}
