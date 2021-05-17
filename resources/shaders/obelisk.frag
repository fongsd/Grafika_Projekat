#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 aNormal;

uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec3 viewPos;

struct Material
{
    vec3 specular;
    vec3 diffuse;
    vec3 ambient;
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
uniform Material material;

vec3 calculateSpotLight(SpotLight spotLight, vec3 fragPos, vec3 viewPos, vec3 norm);

void main()
{

    vec3 ambient = lightColor *  material.ambient;

    vec3 norm = normalize(aNormal);

    vec3 lightDir = normalize(viewPos - fragPos);

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse =(diff * material.diffuse) * lightColor;

    vec3 spotLightVec = calculateSpotLight(spotLight, fragPos, viewPos, norm);

    FragColor = vec4(ambient + diffuse + spotLightVec, 1.0f) * vec4(objectColor, 1.0f);

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

    return spot * lightColor;
}