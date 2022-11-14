#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 vcolor;
layout (location = 4) in float weight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_normal;
out vec2 v_uv;
out vec3 v_vcolor;
out float v_weight;

void main()
{
    mat4 mvp = projection * view * model;
    v_normal = (model * vec4(normal, 0)).xyz;
    v_uv = uv;
    v_vcolor = vcolor;
    v_weight = weight;
    gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0);
}

