#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    float shininess;
};

uniform Material material;

void main() {
  gPosition = FragPos;
  gNormal = normalize(Normal);
  gAlbedoSpec.rgb = texture(material.texture_diffuse0, TexCoords).rgb;
  gAlbedoSpec.a = texture(material.texture_specular0, TexCoords).r;
}