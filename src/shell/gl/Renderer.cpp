//
// Created by solarliner on 12/11/22.
//

#include "Renderer.h"
#include "../UniformBindings.h"
#include "../components/PanOrbitCamera.h"
#include "Camera.h"
#include "ClearColor.h"
#include "Mesh.h"
#include "ProgramRef.h"
#include "Transform.h"
#include <glow/shaders/Program.h>

static void set_size(GLint w, GLint h) { glViewport(0, 0, w, h); }
static void set_size(unsigned int w, unsigned int h) { set_size((GLint) w, (GLint) h); }
static void set_size(glm::vec2 size) { set_size((GLint) size.x, (GLint) size.y); }

namespace shell::gl {
    void Renderer::handle_resize(events::Resize resize) { size_changed = resize.size; }

    void Renderer::before_run(Shell &shell) {
        shell.resources().get<entt::dispatcher>().sink<events::Resize>().connect<&Renderer::handle_resize>(*this);
        auto &camera = shell.resources().get<Camera>();
        auto size = shell.viewport_size();
        set_size(size);
        camera.resize(size);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        lights.bind_to(UniformBindings::Lights);
    }

    void Renderer::execute(Shell &shell) {
        Color clear_color;

        auto &registry = shell.registry;
        auto &camera = registry.ctx().get<Camera>();
        auto &controller = registry.ctx().get<components::PanOrbitCamera>();
        if (registry.ctx().contains<ClearColor>()) clear_color = registry.ctx().get<ClearColor>().color;
        else
            clear_color = registry.ctx().insert_or_assign(ClearColor{Color::Black}).color;
        auto drawables = registry.view<const Transform, const Mesh, const ProgramRef>();

        if (size_changed) {
            spdlog::debug("[Renderer] viewport new size {}x{}", size_changed->x, size_changed->y);
            camera.resize(*size_changed);
            set_size(*size_changed);
            size_changed.reset();
        }

        {
            auto view = registry.view<components::Light>();
            auto map = lights.map(Write);
            auto it = view.begin();
            int i = 0;
            for (; i < Lights::MAX_LIGHTS && it != view.end(); ++i, ++it) {
                map[0].lights[i] = registry.get<components::Light>(*it);
            }
            map[0].num_lights = i;
        }

        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClearDepth(1.f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto [entity, model, mesh, material]: drawables.each()) {
            const auto &program = material.get_program();
            program.use();
            program.set_uniform("cam_pos", controller.eye);
            program.set_uniform("model", model.matrix);
            mesh.draw(registry.any_of<wireframe>(entity));
        }

        shell.swap_buffers();
    }
}// namespace shell::gl