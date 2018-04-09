#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    sampler2D texture_normal0;
    float shininess;
};

uniform Material material;
uniform sampler2D GrabTexture;
uniform vec3 viewPos;
uniform float _Time;
uniform vec2 GrabTextureSize;

void main() {
    // FIX: viewport size
    // vec2 GrabCoords = gl_FragCoord.xy / vec2(1280.0f, 720.0f);
    // vec4 color = vec4(texture(GrabTexture, GrabCoords).rgb, 1.0f);
    // FragColor = 1 - color;

    vec3 viewDir = normalize(viewPos - FragPos);

    float t = _Time / 20.0f;
    vec2 speed = t * vec2(0.5, 0.5);

    vec3 n1 = texture(material.texture_normal0, TexCoords + speed).rgb;
    vec3 n2 = texture(material.texture_normal0, TexCoords - speed).rgb;
    vec3 normal = normalize(n1 + n2);
    normal = normalize(normal * 2.0f - 1.0f);

    vec2 offset = normal.xy * 0.01f * GrabTextureSize;
    vec2 GrabPos = offset + gl_FragCoord.xy;
    vec3 refrColor = texture(GrabTexture, GrabPos / GrabTextureSize).rgb;

    normal = normalize(TBN * normal);

    vec3 texColor = texture(material.texture_diffuse0, TexCoords + speed).rgb;
    float fresnel = pow(1 - max(dot(viewDir, normal), 0.0f), 4);
    vec3 color = texColor * fresnel + refrColor * (1 - fresnel);

    FragColor = vec4(color, 1.0f);
}