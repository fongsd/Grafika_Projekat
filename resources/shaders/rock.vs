#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;
out vec3 Normal;
uniform mat4 projection;
uniform mat4 view;
out vec3 fragPos;
void main()
{
    Normal = transpose(inverse(mat3(aInstanceMatrix))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f);
    fragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0f));
}