#version 330 core

in vec2 texCords;
in vec3 aNormal;
in vec3 fragPos;

uniform sampler2D diffuse_texture1;

uniform vec3 sunLightColor;
uniform vec3 sunLightDirection;
uniform vec3 lightPosition;
out vec4 fragColor;

void main()
{
    vec3 norm = normalize(aNormal);
    vec3 lightDir = normalize(lightPosition - fragPos);

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff *  vec3(texture(diffuse_texture1, texCords));

    //sunLight
    vec3 sunLightDir = normalize(sunLightDirection - fragPos);
    float sunDiff = max(dot(-sunLightDir, norm), 0.0);
    vec3 sunDiffuse = sunDiff * vec3(texture(diffuse_texture1, texCords));

    float ambientStrength = 0.3;
    fragColor = vec4(sunDiffuse + diffuse, 1.0f) ;
}