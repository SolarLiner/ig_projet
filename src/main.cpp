#include "base/Mesh.h"
#include "shell/Shell.h"
#include "shell/events.h"
#include "shell/gl/Camera.h"
#include "shell/gl/PickingSystem.h"
#include "shell/gl/Renderer.h"
#include "shell/gl/Transform.h"
#include "shell/systems/EventListener.h"
#include "shell/systems/OpenMeshUpload.h"
#include "shell/systems/PanOrbitSystem.h"
#include "shell/systems/System.h"
#include "transforms/Laplace.h"

#include <filesystem>
#include <nfd.hpp>
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

entt::entity add_ui(Shell &shell, gl::Renderer::ui_func_t ui) {
    auto entity = shell.registry.create();
    shell.registry.emplace<gl::Renderer::ui_func_t>(entity, ui);
    return entity;
}

int main() {
    spdlog::cfg::load_env_levels();
    transforms::Laplace laplace(0.5);
    Shell shell;
    shell.init();
    shell.setup_default_environment();
    auto &camera = shell.resources().get<gl::Camera>();
    camera.set_fov(0.7f);
    NFD_Init();

    auto start = std::chrono::steady_clock::now();
    shell.emplace_system<systems::PanOrbitSystem>();
    shell.emplace_system<systems::EventListener<events::KeyboardEvent>>(quit_on_escape);
    shell.emplace_system<systems::OpenMeshUpload>("resources");
    //    shell.add_system([laplace](Shell &shell) {
    //        auto view = shell.registry.template view<base::Mesh>();
    //        for (auto entity: view) { shell.registry.patch<base::Mesh>(entity, laplace); }
    //    });
    shell.emplace_system<gl::PickingSystem>("resources");
    shell.emplace_system<gl::Renderer>();

    //    auto mesh_entity = shell.registry.create();
    //    shell.registry.emplace<base::Mesh>(mesh_entity, base::Mesh::open("suzanne.obj"));
    //    shell.registry.emplace<Transform>(mesh_entity);
    //    shell.registry.emplace<std::string>(mesh_entity, "Suzanne");

    auto ambient = shell.registry.create();
    shell.registry.emplace<components::Light>(ambient, components::Light::ambient(Color::White.with_alpha(0.1f)));

    auto dir1 = shell.registry.create();
    shell.registry.emplace<components::Light>(
            dir1, components::Light::directional_light(vec3(1, 1, 1), Color::White.with_alpha(2.f)));

    auto pt1 = shell.registry.create();
    shell.registry.emplace<components::Light>(
            pt1, components::Light::point_light(vec3(1, -1, -1) * 2.f, Color::Green.with_alpha(3.0f)));

    add_ui(shell, [](Shell &shell) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open")) {
                    nfdchar_t *out_path;
                    nfdfilteritem_t items[1] = {{"Mesh files", "obj,off"}};
                    auto result = NFD_OpenDialog(&out_path, items, 1, nullptr);
                    if (result == NFD_OKAY) {
                        std::filesystem::path file(out_path);
                        NFD_FreePath(out_path);
                        auto entity = shell.registry.create();
                        shell.registry.emplace<base::Mesh>(entity, base::Mesh::open(file));
                        shell.registry.emplace<Transform>(entity);
                        shell.registry.emplace<std::string>(entity, file.filename().c_str());
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    });
    add_ui(shell, [laplace](Shell &shell) {
        static std::optional<entt::entity> selected_entity = std::nullopt;
        auto view = shell.registry.view<const std::string, const base::Mesh, Transform>();
        auto selected_label =
                selected_entity && view.contains(*selected_entity) ? view.get<const std::string>(*selected_entity) : "";
        ImGui::Begin("Meshes");
        if (ImGui::BeginCombo("Mesh", selected_label.c_str())) {
            for (auto entity: view) {
                auto &label = view.get<const std::string>(entity);
                const bool is_selected = selected_entity == entity;
                if (ImGui::Selectable(label.c_str(), is_selected)) selected_entity = entity;
                if (is_selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        if (selected_entity) {
            if (ImGui::TreeNode("Transforms")) {
                 static int iterations = 10;
                ImGui::Text("Laplace smoothing");
                ImGui::InputInt("Iterations", &iterations);
                if (ImGui::Button("Apply")) {
                    shell.registry.patch<base::Mesh>(*selected_entity, [laplace](base::Mesh &mesh) {
                        for (int i = 0; i < iterations; ++i) laplace(mesh);
                    });
                }
                ImGui::TreePop();
            }
            if(ImGui::Button("Delete")) {
                shell.registry.remove<std::string>(*selected_entity);
                shell.registry.remove<base::Mesh>(*selected_entity);
                selected_entity.reset();
            }
        }
        ImGui::End();
    });

    shell.run();
}
