#version 330 core
out vec4 FragColor;
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

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

uniform sampler2D shadowMap;
uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;


// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow);
float CalcShadow(vec4 fragPosLightSpace, float bias);

void main() {
    vec3 color = texture(material.texture_diffuse0, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0f);

    vec3 ambient = 0.15 * color;
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
    vec3 specular = spec * lightColor;

    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);
    float shadow = CalcShadow(fs_in.FragPosLightSpace, bias);
    vec3 result = (ambient + (1.0f - shadow) * (specular + diffuse)) * color;

    FragColor = vec4(result, 1.0f);
}

float CalcShadow(vec4 fragPosLightSpace, float bias) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;
    if (projCoords.z > 1.0f) {
        return 0.0f;
    }
    float closetDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    // float shadow = currentDepth - bias > closetDepth ? 1.0f : 0.0f;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}
/*
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0f);

    // vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(viewDir, halfwayDir), 0.0f), material.shininess);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);

    vec3 ambient = vec3(texture(material.texture_diffuse0, TexCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.texture_diffuse0, TexCoords)) * diff * light.diffuse;
    vec3 specular = vec3(texture(material.texture_specular0, TexCoords)) * spec * light.specular;

    return (ambient + diffuse + specular);
}
*/