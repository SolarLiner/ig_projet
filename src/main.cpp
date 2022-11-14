#include "base/Mesh.h"
#include "shell/EventListener.h"
#include "shell/OpenMeshUpload.h"
#include "shell/Renderer.h"
#include "shell/Shell.h"
#include "shell/events.h"
#include "shell/gl/Camera.h"
#include "shell/gl/Mesh.h"
#include "shell/gl/Transform.h"
#include "transforms/Laplace.h"
#include <filesystem>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>

using namespace shell;
using namespace shell::gl;
using namespace shell::gl::resource;
using Key = sf::Keyboard::Key;
namespace fs = std::filesystem;
using glm::mat4;
using glm::quat;
using glm::vec3;
using glm::vec4;

void quit_on_escape(entt::registry &reg, events::KeyboardEvent event) {
    if (event.key == Key::Escape) reg.ctx().template get<entt::dispatcher>().template enqueue<events::Close>();
}

const vec3 base_cam_pos{0.f, 1.f, 3.f};

int main() {
    Shell shell;
    transforms::Laplace laplace(0.5);

    auto &ctx = shell.setup_default_environment();
    auto &camera = ctx.get<Camera>();
    camera.fov = 1.f;
    camera.view() = glm::lookAt(base_cam_pos, vec3(0), vec3(0, 1, 0));

    auto start = std::chrono::steady_clock::now();
    shell.add_system(new EventListener<events::KeyboardEvent>(quit_on_escape));
    shell.add_system(new systems::OpenMeshUpload("resources"));
        shell.add_system([start](const auto &, entt::registry &registry) {
            auto &camera = registry.ctx().get<Camera>();
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = now - start;
            auto angle = elapsed.count();
            vec3 p = glm::angleAxis(angle/3.f, vec3(0, 1, 0)) * base_cam_pos;
            std::cout << glm::to_string(p) << std::endl;
            camera.view() = glm::lookAt(p, vec3(0), vec3(0, 1, 0));
        });
//    shell.add_system([laplace](const auto &, entt::registry &registry) {
//        auto view = registry.view<base::Mesh>();
//        for (const auto entity: view) {
//            registry.patch<base::Mesh>(entity, [&laplace](auto &mesh) { laplace(mesh); });
//        }
//    });
    shell.add_system(new Renderer());

    auto mesh_entity = shell.registry.create();
    shell.registry.emplace<base::Mesh>(mesh_entity, base::Mesh::open("suzanne.obj"));
    shell.registry.emplace<Transform>(mesh_entity);
    shell.registry.emplace<Renderer::wireframe>(mesh_entity);

    shell.run();
}
