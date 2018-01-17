#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    sampler2D texture_normal0;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
};


uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;

uniform mat4 model;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = texture(material.texture_normal0, fs_in.TexCoords).rgb;
    norm = normalize(norm * 2.0f - 1.0f);
    norm = normalize(fs_in.TBN * norm);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

     vec3 result = CalcDirLight(dirLight, norm, viewDir);
      for (int i = 0; i < NR_POINT_LIGHTS; i++) {
          result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
      }

    result += CalcSpotLight(spotLight, norm, fs_in.FragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 ambient = vec3(texture(material.texture_diffuse0,fs_in.TexCoords)) * light.ambient;

    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = vec3(texture(material.texture_diffuse0, fs_in.TexCoords)) * diff * light.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = vec3(texture(material.texture_specular0, fs_in.TexCoords)) * spec * light.specular;

    return ambient + (diffuse + specular) * intensity;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    vec3 ambient = vec3(texture(material.texture_diffuse0, fs_in.TexCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.texture_diffuse0, fs_in.TexCoords)) * diff * light.diffuse;
    vec3 specular = vec3(texture(material.texture_specular0, fs_in.TexCoords)) * spec * light.specular;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 ambient = vec3(texture(material.texture_diffuse0, fs_in.TexCoords)) * light.ambient;

    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = vec3(texture(material.texture_diffuse0, fs_in.TexCoords)) * diff * light.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular0, fs_in.TexCoords)) * spec * light.specular;

    float distances = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distances + light.quadratic * distances * distances);

    return (ambient + diffuse + specular) * attenuation;
}

