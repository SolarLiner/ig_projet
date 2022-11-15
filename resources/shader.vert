#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 vcolor;
layout (location = 4) in float weight;

layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
};
uniform mat4 model;

out Fragment {
    vec3 normal;
    vec2 uv;
    vec3 vcolor;
    float weight;
} frag;

void main()
{
    mat4 mvp = projection * view * model;
    frag.normal = (model * vec4(normal, 0)).xyz;
    frag.uv = uv;
    frag.vcolor = vcolor;
    frag.weight = weight;
    gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0);
}

