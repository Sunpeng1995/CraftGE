#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_LIGHTS 128
uniform PointLight pointLights[NR_LIGHTS];
uniform vec3 viewPos;

void main() {
  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb;
  vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
  float Specular = texture(gAlbedoSpec, TexCoords).a;

  vec3 light = Albedo * 0.1;
  vec3 viewDir = normalize(viewPos - FragPos);

  for (int i = 0; i < NR_LIGHTS; i++) {
    vec3 lightDir = normalize(pointLights[i].position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * pointLights[i].diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * Specular * pointLights[i].specular;

    float distance = length(pointLights[i].position - FragPos);
    float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);

    diffuse *= attenuation;
    specular *= attenuation;
    
    light += diffuse + specular;
  }

  FragColor = vec4(light, 1.0);
}