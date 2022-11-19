#include "base/Mesh.h"
#include "shell/Shell.h"
#include "shell/events.h"
#include "shell/gl/Camera.h"
#include "shell/gl/Renderer.h"
#include "shell/gl/Transform.h"
#include "shell/systems/EventListener.h"
#include "shell/systems/OpenMeshUpload.h"
#include "shell/systems/PanOrbitSystem.h"
#include "shell/systems/System.h"
#include "transforms/Laplace.h"
#include <filesystem>
#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>

using namespace shell;
using namespace shell::gl;
using glm::mat4;
using glm::quat;
using glm::vec3;
using glm::vec4;

void quit_on_escape(entt::registry &reg, events::KeyboardEvent event) {
    if (event.is(SDLK_ESCAPE)) reg.ctx().template get<entt::dispatcher>().template enqueue<events::Close>();
}

int main() {
    spdlog::cfg::load_env_levels();
    transforms::Laplace laplace(0.5);
    Shell shell;
    shell.init();
    shell.setup_default_environment();
    auto &camera = shell.resources().get<gl::Camera>();
    camera.set_fov(0.7f);

    auto start = std::chrono::steady_clock::now();
    shell.emplace_system<systems::PanOrbitSystem>();
    shell.emplace_system<systems::EventListener<events::KeyboardEvent>>(quit_on_escape);
    shell.emplace_system<systems::OpenMeshUpload>("resources");
    shell.add_system([laplace](Shell &shell) {
        auto view = shell.registry.template view<base::Mesh>();
        for (auto entity: view) { shell.registry.patch<base::Mesh>(entity, laplace); }
    });

    shell.emplace_system<gl::Renderer>();

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
