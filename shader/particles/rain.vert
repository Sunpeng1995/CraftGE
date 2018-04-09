#version 330 core
layout (location = 0) in vec3 quadVertices;
layout (location = 1) in vec4 xyzs;
layout (location = 2) in vec4 aColors;

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
            + camera_right * quadVertices.x * psize * 0.05f
            + camera_up * quadVertices.y * psize * 5.0f;
    
    gl_Position = projection * view * vec4(vertex_position, 1.0f);
    Texcoords = quadVertices.xy + vec2(0.5f, 0.5f);
}