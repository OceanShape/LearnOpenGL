#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D block;
uniform sampler2D mario;

void main()
{
   FragColor = mix(texture(block, TexCoord), texture(mario, TexCoord), 0.5);
}
