#version 330 core
in vec4 Colors;
in vec2 Texcoords;
out vec4 FragColor;

uniform sampler2D particle_texture;

void main() {
    FragColor = texture(particle_texture, Texcoords) * Colors;
}