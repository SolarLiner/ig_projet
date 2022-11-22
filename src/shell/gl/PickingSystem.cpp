//
// Created by solarliner on 20/11/22.
//

#include "PickingSystem.h"

#include "../Shell.h"
#include "Mesh.h"

namespace shell::gl {
    PickingSystem::PickingSystem(const std::filesystem::path &resources_dir)
        : PickingSystem(Shader(glow::shaders::Shader::Vertex, resources_dir / "select.vert"),
                        Shader(glow::shaders::Shader::Fragment, resources_dir / "select.frag")) {}

    PickingSystem::PickingSystem(Shader vs, Shader fs) : program() {
        program.add_shader(vs);
        program.add_shader(fs);
        program.link();
    }

    void PickingSystem::before_run(Shell &shell) {
        auto &dispatcher = shell.resources().get<entt::dispatcher>();
        dispatcher.sink<events::Resize>().connect<&PickingSystem::handle_resize>(*this);
        dispatcher.sink<events::MouseButton>().connect<&PickingSystem::handle_click>(*this);

        auto size = shell.viewport_size();
        pick_texture.clear_resize((size_t) size.x, (size_t) size.y);
        pick_depth.clear_resize((size_t) size.x, (size_t) size.y);

        framebuffer.attach_color(pick_texture, 0);
        framebuffer.attach_depth(pick_depth);
    }

    void PickingSystem::execute(Shell &shell) {
//        if (!do_pick) return;
        std::map<unsigned int, entt::entity> ids{};
        auto &camera = shell.resources().get<gl::Camera>();
        auto view = shell.registry.view<const Transform, const gl::Mesh>();
        unsigned int i = 0;
        framebuffer.bind();
        program.use();
        glClearColor(-1.f, 0.f, 0.f, 1.f);
        glClearDepth(1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto [entity, transform, mesh]: view.each()) {
            ids.insert_or_assign(i, entity);
            camera.bind_to(0);
            program.set_uniform_block("Camera", 0);
            program.set_uniform("model", transform.matrix);
            program.set_uniform("object_id", i++);
            mesh.draw();
        }
        framebuffer.unbind();

        auto res = pick_texture.read_pixel((size_t) do_pick->x, (size_t) do_pick->y);
        auto id = res.at(0);
        auto vert = res.at(1);
        if (id > 0) {
            spdlog::info("Picked item ID {} @ {}x{}", id, do_pick->x, do_pick->y);
            spdlog::info("Picked vertex ID {}", vert);
            shell.resources().get<entt::dispatcher>().enqueue<events::Pick>(*do_pick, ids[id]);
        }
        do_pick.reset();
    }

    void PickingSystem::handle_resize(events::Resize resize) {
        pick_texture.clear_resize((size_t) resize.size.x, (size_t) resize.size.y);
        pick_depth.clear_resize((size_t) resize.size.x, (size_t) resize.size.y);
    }
    void PickingSystem::handle_click(events::MouseButton btn) {
        if (btn.button == SDL_BUTTON_LEFT && btn.pressed) { do_pick = btn.at; }
    }
}// namespace shell::gl