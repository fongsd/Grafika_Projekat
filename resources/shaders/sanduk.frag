#version 330 core

in vec2 texCords;
in vec3 aNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D wood_texture;

void main()
{
    fragColor = texture(wood_texture, texCords) * 0.5;
}