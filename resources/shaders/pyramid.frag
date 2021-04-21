#version 330 core

out vec4 fragColor;
in vec2 texCords;
uniform sampler2D texture_pyramid;
//uniform sampler2D t1;

void main()
{
    fragColor = texture(texture_pyramid, texCords);
    //fragColor = vec4(7.0, 4.0, 3.0, 1.0);
}