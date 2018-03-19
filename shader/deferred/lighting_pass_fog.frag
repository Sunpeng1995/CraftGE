#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform float utime;
uniform sampler2D fogNoise;
float fogStart = 0;
float fogEnd = 1.5;

void main() {
  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb;
  vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
  float Specular = texture(gAlbedoSpec, TexCoords).a;
  
  float noise = texture(fogNoise, TexCoords + vec2(utime, utime)).r - 0.5;

  vec3 light = Albedo * 0.1;
  vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lightDir = normalize(-dirLight.direction);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * dirLight.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * Specular * dirLight.specular;

    light += diffuse + specular;
    // float d = sqrt((viewDir.x - FragPos.x) *(viewDir.x - FragPos.x) + (viewDir.z - FragPos.z) *(viewDir.z - FragPos.z));

  float fogDensity = (fogEnd - FragPos.y) / (fogEnd - fogStart);
  // float fogDensity = (d - fogStart) / (fogEnd - fogStart);
  fogDensity = clamp(fogDensity * (1 + noise), 0.0, 1.0);
  light = light * (1 - fogDensity) + vec3(1.0f) * fogDensity;

  FragColor = vec4(light, 1.0);
}