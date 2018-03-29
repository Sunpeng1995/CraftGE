#version 330 core
layout (location = 0) in vec3 quadVertices;
layout (location = 1) in vec4 xyzs;
layout (location = 2) in vec4 aColors;
layout (location = 3) in float period;

out vec2 Texcoords;

uniform vec3 camera_right;
uniform vec3 camera_up;
uniform mat4 view;
uniform mat4 projection;

void main() {
    float psize = xyzs.w;
    vec3 position = xyzs.xyz;
    vec3 vertex_position = 
            position 
            + camera_right * quadVertices.x// * psize 
            + camera_up * quadVertices.y;// * psize;
    
    gl_Position = projection * view * vec4(vertex_position, 1.0f);
    // int x = period % 8;
    // int y = period / 8;
    float y = floor(period * 0.125f);
    float x = floor(period - y * 8.0f);
    y = 7 - y;
    Texcoords.x = ((quadVertices.x + 0.5f) + x) * 0.125f;
    Texcoords.y = ((quadVertices.y + 0.5f) + y) * 0.125f;
}