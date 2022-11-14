#version 330 core
out vec4 out_color;
in vec3 v_normal;
in vec2 v_uv;
in vec3 v_vcolor;
in float v_weight;

const vec3 light_dir = normalize(vec3(1., 1., 1.));

vec3 tonemap(vec3 x) {
    return x / (1.0 + x);
}

void main()
{
    float k = max(0., dot(light_dir, v_normal)) + 0.1;
    vec3 color = tonemap(k * vec3(0.3, 0.6, 0.9));
    out_color = vec4(color, 1.f);
}
