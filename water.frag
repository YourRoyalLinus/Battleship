#version 460 core
#define PI 3.14159265358979323846
#define TAU (2 * PI)
#define SCREEN_WIDTH 1200
out vec4 FragColor;

in vec2 TexCoord;
//uniform vec3 iResolution;
uniform float iTime;
uniform sampler2D image;
uniform sampler2D waveMap;

void main()
{
    //TODO: FIX UVS!
    vec2 uv = vec2((gl_FragCoord.x - 600)/600.0 , gl_FragCoord.y/600.0);
    //vec2 uv = TexCoord;
                   
   	//background color rgb( 49/255, 169/255, 238/255, 255/255 ) -- 0.192156862745098, 0.6627450980392157, 0.9333333333333333
    //vec4 texture_color = vec4(0.192156862745098, 0.6627450980392157, 0.9333333333333333, 1.0);

    vec4 texture_color = texture(image, TexCoord);
    vec4 waveMap = texture(waveMap, vec2(uv.x, uv.y) + iTime/30);
    //vec4 k = vec4(1.0);
    //vec4 k = vec4(iTime)*0.8;
	//k.xy = uv * 6.0;
    //float val1 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
    //float val2 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.2));
    //float val3 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
    //vec4 color = vec4 ( pow(min(min(val1,val2),val3), 7.0) * 3.0) + texture_color;
    FragColor = texture_color + (waveMap * .3);
    //FragColor = clamp(texture_color * (waveMap + 1.0), 0.0, 1.0);
}






