#version 330 core

out vec4 fragColor;
in vec2 texCords;
uniform sampler2D texture_sand;
uniform vec3 lightColor;
uniform vec3 lightPosition;
//uniform sampler2D t1;

in vec3 fragPos;
void main()
{
    vec3 norm = vec3(0, 1, 0);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = lightColor * ambientStrength;

    fragColor = vec4(diffuse + ambient , 1.0f)  * texture(texture_sand, texCords);
}