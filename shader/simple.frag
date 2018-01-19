#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixer;

void main() {
	vec2 faceCoord = vec2(1.0f - TexCoord.x, TexCoord.y);
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, faceCoord), mixer);
}