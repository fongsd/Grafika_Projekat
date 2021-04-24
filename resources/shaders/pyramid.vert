#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCords;
layout (location = 2) in vec3 normal;

out vec3 aNormal;

out vec2 texCords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    aNormal = normal;
    //gl_Position = vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    texCords = aTexCords;
}