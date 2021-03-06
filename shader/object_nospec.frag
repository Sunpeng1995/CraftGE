#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    float alpha = texture(material.texture_diffuse0, TexCoords).a;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

     vec3 result = CalcDirLight(dirLight, norm, viewDir);
       for (int i = 0; i < NR_POINT_LIGHTS; i++) {
           result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
       }

     result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, alpha);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 ambient = vec3(texture(material.texture_diffuse0,TexCoords)) * light.ambient;

    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = vec3(texture(material.texture_diffuse0, TexCoords)) * diff * light.diffuse;

    // vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(viewDir, halfwayDir), 0.0f), 32);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32);
    vec3 specular = vec3(texture(material.texture_specular0, TexCoords)) * spec * light.specular;

    return ambient + (diffuse) * intensity;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0f);

    // vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(viewDir, halfwayDir), 0.0f), material.shininess);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);

    vec3 ambient = vec3(texture(material.texture_diffuse0, TexCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.texture_diffuse0, TexCoords)) * diff * light.diffuse;

    return ambient + diffuse;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 ambient = vec3(texture(material.texture_diffuse0, TexCoords)) * light.ambient;

    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = vec3(texture(material.texture_diffuse0, TexCoords)) * diff * light.diffuse;

    // vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(viewDir, halfwayDir), 0.0f), material.shininess);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular0, TexCoords)) * spec * light.specular;

    float distances = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distances + light.quadratic * distances * distances);

    return (ambient + diffuse) * attenuation;
}

