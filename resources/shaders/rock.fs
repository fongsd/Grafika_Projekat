#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
struct DirLight
{
    vec3 direction;
    vec3 color;
};

in vec3 fragPos;
uniform DirLight dirLight;
vec3 calcDifLight(DirLight dirLight, vec3 fragPos, vec3 viewPos, vec3 normals);

void main()
{
    vec3 normals = normalize(Normal);

    vec3 diffuse = calcDifLight(dirLight, fragPos, viewPos, normals);

    FragColor = vec4(diffuse, 1.0f) * texture(texture_diffuse1, TexCoords);
}

vec3 calcDifLight(DirLight dirLight, vec3 fragPos, vec3 viewPos, vec3 normals)
{
    vec3 lightDir = normalize(dirLight.direction - fragPos);

    float diffuse = max(dot(lightDir, normals), 0.0);
    vec3 diffuseVec = diffuse * dirLight.color;
    return diffuseVec;

}