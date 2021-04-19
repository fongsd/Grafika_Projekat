#version 330 core

out vec4 fragColor;

//in vec2 texCords;
//uniform sampler2D t0;
//uniform sampler2D t1;

void main()
{
    //fragColor = mix(texture(t0, texCords),texture(t1, texCords), 0.5) ;
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}