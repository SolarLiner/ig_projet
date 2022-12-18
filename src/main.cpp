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
#include "transforms/Deform.h"

#include <filesystem>
#include <nfd.hpp>
#include <spdlog/cfg/env.h>

using namespace shell;
using namespace shell::gl;
using glm::mat4;
using glm::quat;
using glm::vec3;
using glm::vec4;

static entt::entity selected_entity = entt::null;
static size_t selected_vert = 0;


void quit_on_escape(entt::registry &reg, events::KeyboardEvent event) {
    if (event.is(SDLK_ESCAPE))
        reg.ctx().template get<entt::dispatcher>().
                template enqueue<events::Close>();
}

entt::entity add_ui(Shell &shell, gl::Renderer::ui_func_t ui) {
    auto entity = shell.registry.create();
    shell.registry.emplace<gl::Renderer::ui_func_t>(entity, ui);
    return entity;
}

void open_mesh(Shell &shell, const std::filesystem::path &file) {
    auto entity = shell.registry.create();
    auto &mesh = shell.registry.emplace<base::Mesh>(entity, base::Mesh::open(file));
    shell.registry.emplace<Transform>(entity);
    shell.registry.emplace<std::string>(entity, file.filename().c_str());

    mesh.reset_vertex_colors();

    selected_entity = entity;
}

int main(int argc, char **argv) {
    spdlog::cfg::load_env_levels();
    transforms::Laplace laplace(0.5);
    transforms::Deform deform{{0, 0, 1}, 0.5f};
    Shell shell;
    shell.init();
    shell.setup_default_environment();
    auto &camera = shell.resources().get<gl::Camera>();
    camera.set_fov(0.8f);
    NFD_Init();

    shell.emplace_system<systems::PanOrbitSystem>();
    shell.emplace_system<systems::EventListener<events::KeyboardEvent>>(quit_on_escape);
    shell.emplace_system<systems::OpenMeshUpload>("resources");
    shell.emplace_system<gl::Renderer>();

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
                    nfdfilteritem_t items[1] = {{"Mesh files", "obj,off,stl,ply"}};
                    auto result = NFD_OpenDialog(&out_path, items, 1, nullptr);
                    if (result == NFD_OKAY) {
                        std::filesystem::path file(out_path);
                        NFD_FreePath(out_path);
                        open_mesh(shell, file);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    });
    add_ui(shell, [&laplace, &deform](Shell &shell) {
        auto view = shell.registry.view<std::string, const base::Mesh, Transform>();
        auto selected_label =
                shell.registry.valid(selected_entity) && view.contains(selected_entity) ? view.get<const std::string>(
                        selected_entity) : "";

        ImGui::Begin("Object menu");
        if (ImGui::BeginCombo("Object", selected_label.c_str())) {
            for (auto entity: view) {
                auto &label = view.get<const std::string>(entity);
                const bool is_selected = selected_entity == entity;
                if (ImGui::Selectable(label.c_str(), is_selected)) selected_entity = entity;
                if (is_selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        if (shell.registry.valid(selected_entity)) {
            auto &label = view.get<std::string>(selected_entity);
            ImGui::InputText("Name", label.data(), label.size());
            if (ImGui::TreeNode("Move")) {
                auto &model = view.get<Transform>(selected_entity).matrix;
                ImGui::InputFloat3("Position", glm::value_ptr(model) + 12);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Operators")) {
                if (ImGui::BeginTabBar("operators")) {
                    if (ImGui::BeginTabItem("Laplace smoothing")) {
                        static int iterations = 10;
                        ImGui::SliderFloat("Alpha", &laplace.alpha, 0.f, 0.5f);
                        ImGui::InputInt("Iterations", &iterations);
                        if (ImGui::Button("Apply")) {
                            shell.registry.patch<base::Mesh>(selected_entity, [laplace](base::Mesh &mesh) {
                                for (int i = 0; i < iterations; ++i) laplace(mesh);
                            });
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Deformation")) {
                        std::stringstream select_s;
                        select_s << "Vertex #" << selected_vert;
                        ImGui::Text("Deformation");
                        if (ImGui::BeginCombo("Vertex", select_s.str().c_str())) {
                            const auto &mesh = view.get<const base::Mesh>(selected_entity);

                            for (size_t i = 0; i < mesh.n_vertices(); ++i) {
                                std::stringstream label_s;
                                label_s << "Vertex #" << i;
                                const bool is_selected = selected_vert == i;
                                if (ImGui::Selectable(label_s.str().c_str(), is_selected)) selected_vert = i;
                                if (is_selected) ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::InputFloat("Max distance", &deform.max_distance);
                        ImGui::InputFloat3("Direction", glm::value_ptr(deform.dir));
                        if (ImGui::Button("Preview")) {
                            shell.registry.patch<base::Mesh>(selected_entity, [&deform](auto &mesh) {
                                auto handle = mesh.vertex_handle(selected_vert);
                                deform.preview_weight_in_color(mesh, handle);
                            });
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Deform")) {
                            shell.registry.patch<base::Mesh>(selected_entity, [&deform](base::Mesh &mesh) {
                                mesh.request_vertex_status();
                                auto handle = mesh.vertex_handle(selected_vert);
                                deform(mesh, handle);
                                mesh.release_vertex_status();
                            });
                        }
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }

                ImGui::TreePop();
            }

            ImGui::NewLine();
            if (ImGui::Button("Reset vertex colors")) {
                shell.registry.patch<base::Mesh>(selected_entity, [](auto &mesh) {
                    mesh.reset_vertex_colors();
                });
            }
            if (ImGui::Button("Delete")) {
                shell.registry.destroy(selected_entity);
                selected_entity = entt::null;
            }
        }
        ImGui::End();
    });

    for (size_t i = 1; i < argc; ++i) {
        std::filesystem::path path(argv[i]);
        open_mesh(shell, path);
    }

    shell.run();
}
