#version 460 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 FragColor;

uniform sampler2D image;

void main()
{
    FragColor = (texture(image, TexCoords) * ParticleColor);
}  