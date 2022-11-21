#version 330 core
layout (location = 0) in vec3 position;

layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
};
uniform mat4 model;

void main() {
    mat4 mvp = model * view * projection;
    gl_Position = mvp * vec4(position, 1);
}
