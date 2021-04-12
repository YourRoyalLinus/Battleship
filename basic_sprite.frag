#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D image;
uniform vec4 spriteColor;


void main()
{
    FragColor = vec4(spriteColor) * texture(image, TexCoord);
}