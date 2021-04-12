#version 460 core
#define PI 3.14159265359

out vec4 FragColor;

in vec2 TexCoord;

uniform float time;
uniform float fillStrength;
uniform float radius;
uniform vec2 resolution;
uniform float glowStrength;
uniform vec3 color;
uniform sampler2D image;

float d2y(float d) {
    return 1. / (0.2 + d);
}

float fct(vec2 p, float r) {
    float a = 3.0 * mod(-atan(p.y, p.x) + time, 2.0 * PI);
    return (d2y(a) + fillStrength) * (1.0 - step(radius, r));
}

float circle(vec2 p, float r) {
    float d = distance(r, radius);
    return d2y(100.0 * d);
}


void main(void) {
    vec2 position = ((gl_FragCoord.xy) - 0.5 * resolution) / resolution.y;
    position /= cos(1.5 * length(position));
    float y = 0.0;
    float dc = length(position);
    y += fct(position, dc);
    y += circle(position, dc);
    y = pow(y, 1.0 / glowStrength );
    FragColor = vec4(sqrt(y) * color , 1.0) *  texture(image, TexCoord);;
}
