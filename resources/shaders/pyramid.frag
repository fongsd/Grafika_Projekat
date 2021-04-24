#version 330 core

out vec4 fragColor;
in vec2 texCords;
uniform sampler2D texture_pyramid;
//uniform sampler2D t1;
in vec3 aNormal;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;
void main()
{
    float ambientColor = 0.4;

    vec3 norm = normalize(aNormal);

    vec3 lightDir = normalize(lightPos - norm);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    fragColor = vec4(diffuse + ambientColor, 1.0f)  * texture(texture_pyramid, texCords);
    //fragColor = vec4(7.0, 4.0, 3.0, 1.0);
}