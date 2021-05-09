#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform sampler2D sunGradient;

void main()
{
    //these texture coords are not right at all idk wtf I'm doing
    //this whole shader is dogshit
    vec2 sunPosition = vec2( gl_FragCoord.x/600, -gl_FragCoord.y/600);
    vec4 sunColor = texture(sunGradient,sunPosition);
    vec4 diffuse = texture(image, TexCoord) * sunColor * 1.3;
    vec4 ambient = texture(image,TexCoord) * .7;
    FragColor = diffuse + ambient;
    FragColor.a = texture(image, TexCoord).a;
}