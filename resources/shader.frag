#version 330 core
out vec4 out_color;

in Fragment {
    vec3 normal;
    vec2 uv;
    vec3 vcolor;
    float weight;
} frag;

const vec3 light_dir = normalize(vec3(1., 1., 1.));

vec3 tonemap(vec3 x) {
    return x / (1.0 + x);
}

void main()
{
    float k = max(0., dot(light_dir, frag.normal)) + 0.1;
    vec3 color = tonemap(k * vec3(0.3, 0.6, 0.9));
    out_color = vec4(color, 1.f);
}
