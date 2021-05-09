#version 460 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    fragPos = vec3(vertex.xy, -10.0);
    TexCoord = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0, 1.0);
}
