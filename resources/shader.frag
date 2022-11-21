#version 330 core

#define KIND_POINT 0
#define KIND_DIR 1
#define KIND_AMBIENT 2

// To be synced with Renderer.h
#define MAX_LIGHTS 16

const float PI = 3.14159265358979323846;
const float TAU = 2*PI;

uniform vec3 cam_pos;

out vec4 out_color;

struct Light {
    int kind;
    vec3 pos_dir;
    vec4 color;
};

layout(std140) uniform Lights {
    int num_lights;
    Light lights[MAX_LIGHTS];
};

in Fragment {
    vec3 pos;
    vec3 normal;
    vec2 uv;
    vec3 vcolor;
    float weight;
} frag;

uniform mat4 model;

const float roughness = 0.45;
const float metallic = 0;

vec3 fresnel_shlick(float costheta, vec3 f0) {
    return f0 + (1.0 - f0) * pow(clamp(1.0 - costheta, 0.0, 1.0), 5.0);
}

float ggx(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float schlick_ggx(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = schlick_ggx(NdotV, roughness);
    float ggx1  = schlick_ggx(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 tonemap(vec3 x) {
    return x / (1.0 + x);
}

void main()
{
    vec3 albedo = vec3(1);
    vec3 N = normalize(frag.normal);
    vec3 V = normalize(cam_pos - frag.pos);
    vec3 radiance_tot = vec3(0);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    for (int i = 0; i < min(num_lights, MAX_LIGHTS); ++i) {
        Light light = lights[i];
        if (light.kind == KIND_AMBIENT) {
            radiance_tot += light.color.rgb * light.color.a;
            continue;
        }
        vec3 to_light = light.pos_dir - frag.pos;
        vec3 L = light.kind == KIND_POINT ? normalize(to_light) : light.pos_dir;
        vec3 H = normalize(V+L);
        float l2 = light.kind == KIND_POINT ? dot(to_light, to_light) : 1;
        vec3 radiance = light.color.rgb * light.color.a / l2;

        float ndf = ggx(N, H, roughness);
        float g = smith(N, V, L, roughness);
        vec3 fresnel = fresnel_shlick(max(dot(H, V), 0.0), F0);

        vec3 ks = fresnel;
        vec3 kd = 1 - ks;
        kd *= 1 - metallic;

        vec3 num = ndf * g * fresnel;
        float den = 4 * max(dot(N, V), 0) * max(dot(N, L), 0) + 1e-4;
        vec3 specular = num/den;

        radiance_tot += (kd * albedo / PI + specular) * radiance * max(dot(N, L), 0);
    }
    vec3 color = pow(tonemap(radiance_tot), vec3(1/2.2));
    out_color = vec4(color, 1.f);
}
