#version 460 core
#define PI 3.14159265358979323846
#define TAU (2 * PI)
#define SCREEN_WIDTH 1200
out vec4 FragColor;


//uniform vec3 iResolution;
uniform float iTime;
uniform sampler2D image;

vec4 wave(vec2 uv);

void main()
{
    //TOOD: FIX UVS!
    vec2 uv = vec2((gl_FragCoord.x - 600)/600.0 , gl_FragCoord.y/600.0);
    //texture
    vec4 grid = texture(image, uv);

    grid *= wave(uv);
    grid.a = .4;
    FragColor = grid;
}

vec4 wave(vec2 uv){
    const float WAVE_HEIGHT = 10.0;
    const float WAVE_FREQUENCY = 10.0;
    const float shininess = 5.0;
    const float SPEED = 100;
    float offset = cos((uv.x + uv.y) * TAU * WAVE_HEIGHT) * .05 + iTime/SPEED;
    //float offset = uv.y + sin(PI * 2 * iTime/1000);
    float pattern = cos((1.0-((uv.x + offset))) * TAU * WAVE_FREQUENCY); //I have no fucking clue what this is even doing anymore...
    pattern = pow(pattern, shininess);
   
    return vec4(clamp(pattern, 0.0, 1.0)); //I shouldn't have to clamp this but my uv's are incorrect right now or something idk.
}


