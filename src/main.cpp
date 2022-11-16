#include "base/Mesh.h"
#include "shell/Renderer.h"
#include "shell/Shell.h"
#include "shell/events.h"
#include "shell/gl/Camera.h"
#include "shell/gl/Transform.h"
#include "shell/systems/EventListener.h"
#include "shell/systems/OpenMeshUpload.h"
#include "shell/systems/PanOrbitSystem.h"
#include "transforms/Laplace.h"
#include <filesystem>
#include <string>

using namespace shell;
using namespace shell::gl;
using namespace shell::gl::resource;
using Key = sf::Keyboard::Key;
using glm::mat4;
using glm::quat;
using glm::vec3;
using glm::vec4;

void quit_on_escape(entt::registry &reg, events::KeyboardEvent event) {
    if (event.key == Key::Escape) reg.ctx().template get<entt::dispatcher>().template enqueue<events::Close>();
}

int main() {
//    transforms::Laplace laplace(0.5);
    Shell shell;

    auto &ctx = shell.setup_default_environment();
    auto &camera = ctx.get<gl::Camera>();
    camera.set_fov(0.7f);

    auto start = std::chrono::steady_clock::now();
    shell.add_system(new systems::PanOrbitSystem());
    shell.add_system(new systems::EventListener<events::KeyboardEvent>(quit_on_escape));
    shell.add_system(new systems::OpenMeshUpload("resources"));
/*
    shell.add_system([laplace](const auto &, entt::registry &registry) {
        auto view = registry.template view<base::Mesh>();
        for (auto entity: view) {
            registry.patch<base::Mesh>(entity, laplace);
        }
    });
*/
    shell.add_system(new Renderer());

    auto mesh_entity = shell.registry.create();
    shell.registry.emplace<base::Mesh>(mesh_entity, base::Mesh::open("suzanne.obj"));
    shell.registry.emplace<Transform>(mesh_entity);

    auto ambient = shell.registry.create();
    shell.registry.emplace<components::Light>(ambient, components::Light::ambient(Color::White.with_alpha(0.1f)));

    auto dir1 = shell.registry.create();
    shell.registry.emplace<components::Light>(
            dir1, components::Light::directional_light(vec3(1, 1, 1), Color::White.with_alpha(2.f)));

    auto pt1 = shell.registry.create();
    shell.registry.emplace<components::Light>(
            pt1, components::Light::point_light(vec3(1, -1, -1) * 2.f, Color::Green.with_alpha(3.0f)));

    shell.run();
}
