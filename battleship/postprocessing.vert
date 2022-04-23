#version 460 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform bool  shake;
uniform float time;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f); 
    vec2 _texture = vertex.zw;
    TexCoords = _texture;
    if (shake)
    {
        float strength = 0.002;
        gl_Position.x += cos(time * 100) * strength;        
        gl_Position.y -= cos(time * 150) * strength;        
    }
}  