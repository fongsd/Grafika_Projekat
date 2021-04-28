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
uniform Material material;

void main()
{

    vec3 ambient = lightColor *  material.ambient;

    vec3 norm = normalize(aNormal);

    vec3 lightDir = normalize(viewPos - fragPos);

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse =(diff * material.diffuse) * lightColor;

    FragColor = vec4(ambient + diffuse, 1.0f) * vec4(objectColor, 1.0f);

}