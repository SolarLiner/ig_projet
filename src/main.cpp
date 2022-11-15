#include "base/Mesh.h"
#include "shell/Renderer.h"
#include "shell/Shell.h"
#include "shell/events.h"
#include "shell/gl/Camera.h"
#include "shell/gl/Transform.h"
#include "shell/systems/EventListener.h"
#include "shell/systems/OpenMeshUpload.h"
#include "shell/systems/PanOrbitSystem.h"
#include <filesystem>
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

    auto &ctx = shell.setup_default_environment();
    auto &camera = ctx.get<Camera>();
    camera.set_fov(1.f);
    camera.set_view(glm::lookAt(base_cam_pos, vec3(0), vec3(0, 1, 0)));

    auto start = std::chrono::steady_clock::now();
    shell.add_system(new systems::PanOrbitSystem());
    shell.add_system(new systems::EventListener<events::KeyboardEvent>(quit_on_escape));
    shell.add_system(new systems::OpenMeshUpload("resources"));
    shell.add_system(new Renderer());

    auto mesh_entity = shell.registry.create();
    shell.registry.emplace<base::Mesh>(mesh_entity, base::Mesh::open("suzanne.obj"));
    shell.registry.emplace<Transform>(mesh_entity);

    shell.run();
}
