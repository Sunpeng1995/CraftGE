#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    float shininess;
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

uniform Material material;
uniform PointLight pointLight;

uniform vec3 viewPos;
uniform float far_plane;
uniform samplerCube depthMap;

float CalcShadow(vec3 fragPos);
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

void main() {
    vec3 color = texture(material.texture_diffuse0, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    /////// fix
    vec3 ambient = color * pointLight.ambient;

    vec3 lightDir = normalize(pointLight.position - fs_in.FragPos);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * pointLight.diffuse;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(viewDir, halfwayDir), 0.0f), material.shininess);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);
    vec3 specular = spec * pointLight.specular;

    float distances = length(pointLight.position - fs_in.FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distances + pointLight.quadratic * distances * distances);

    float shadow = CalcShadow(fs_in.FragPos);
    vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0f);
}

float CalcShadow(vec3 fragPos) {
    vec3 fragToLight = fragPos - pointLight.position;
    float closestDepth = texture(depthMap, fragToLight).r;

    //debug
    // FragColor = vec4(vec3(closestDepth), 1.0f);

    closestDepth *= far_plane;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    // float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;


    return shadow;
}