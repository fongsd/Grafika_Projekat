#version 330 core

out vec4 fragColor;
in vec2 texCords;
uniform sampler2D texture_sand;
//uniform sampler2D t1;

void main()
{
    fragColor = texture(texture_sand, texCords) * 0.4;
    //fragColor = vec4(7.0, 4.0, 3.0, 1.0);
}