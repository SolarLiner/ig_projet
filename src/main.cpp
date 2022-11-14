#include "base/Mesh.h"
#include "shell/EventListener.h"
#include "shell/Shell.h"
#include "shell/events.h"
#include "shell/gl/Camera.h"
#include "shell/gl/Mesh.h"
#include "shell/gl/Renderer.h"
#include "shell/gl/Transform.h"
#include "shell/gl/resource/shaders/Program.h"
#include "shell/gl/resource/shaders/Shader.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <glm/gtx/string_cast.hpp>

using namespace shell;
using namespace shell::gl;
using namespace shell::gl::resource;
using Key = sf::Keyboard::Key;
namespace fs = std::filesystem;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;

void quit_on_escape(entt::registry &reg, events::KeyboardEvent event) {
    if (event.key == Key::Escape) reg.ctx().template get<entt::dispatcher>().template enqueue<events::Close>();
}

const vec3 base_cam_pos{0.f, 1.f, 3.f};

int main() {
    Shell shell;
    Shader vs(Shader::Type::Vertex, fs::path("resources/shader.vert"));
    Shader fs(Shader::Type::Fragment, fs::path("resources/shader.frag"));
    base::Mesh openmesh = base::Mesh::open("suzanne.obj");

    auto mesh_entity = shell.registry.create();
    auto &gl_mesh = shell.registry.emplace<Mesh>(mesh_entity);
    auto &shader = shell.registry.emplace<Program>(mesh_entity);
    auto &transform = shell.registry.emplace<Transform>(mesh_entity);
    gl_mesh.update_from_openmesh(openmesh);
    shader.add_shader(vs);
    shader.add_shader(fs);
    shader.link();

    auto &camera = shell.registry.ctx().emplace<Camera>();
    camera.fov = 1.f;
    camera.view() = glm::lookAt(base_cam_pos, vec3(0), vec3(0, 1, 0));

    auto start = std::chrono::steady_clock::now();
    shell.registry.ctx().insert_or_assign(ClearColor{Color(0.1f, 0.1f, 0.1f)});
    shell.add_system(new EventListener<events::KeyboardEvent>(quit_on_escape));
    shell.add_system([start](const auto &, entt::registry &registry) {
        Camera &camera = registry.ctx().get<Camera>();
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - start;
        auto angle = elapsed.count();
        vec3 p = glm::angleAxis(angle, vec3(0, 1, 0)) * base_cam_pos;
        std::cout << glm::to_string(p) << std::endl;
        camera.view() = glm::lookAt(p, vec3(0), vec3(0, 1, 0));
    });
    shell.add_system(new Renderer());
    shell.run();
}
