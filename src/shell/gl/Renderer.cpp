//
// Created by solarliner on 12/11/22.
//

#include "Renderer.h"
#include "../Color.h"
#include "../UniformBindings.h"
#include "../components/Light.h"
#include "../components/PanOrbitCamera.h"
#include "Camera.h"
#include "ClearColor.h"
#include "Mesh.h"
#include "ProgramRef.h"
#include "Transform.h"
#include <glow/shaders/Program.h>

static void set_size(GLint w, GLint h) { glViewport(0, 0, w, h); }
static void set_size(unsigned int w, unsigned int h) { set_size((GLint) w, (GLint) h); }
static void set_size(sf::Vector2u size) { set_size(size.x, size.y); }
static void set_size(glm::vec2 size) { set_size((GLint) size.x, (GLint) size.y); }

#ifdef DEBUG_OPENGL
#include <iostream>
#include <cstdarg>
static void print_opengl_calls(const char *name, void *, int nargs, ...) {
    va_list args;
    va_start(args, nargs);
    std::cout << "[DEBUG] OpenGL: " << name << "(";
    for (int i = 0; i < nargs; ++i) {
        int arg = va_arg(args, int);
        if (i > 0) std::cout << ", ";
        std::cout << arg;
    }
    std::cout << ")" << std::endl;
}
#endif

namespace shell::gl {
    Renderer::Renderer() {

#ifdef DEBUG_OPENGL
        glad_set_pre_callback(print_opengl_calls);
#endif
    }

    void Renderer::handle_resize(events::Resize resize) { size_changed = glm::vec2(resize.width, resize.height); }

    void Renderer::before_run(const sf::Window &window, entt::registry &registry) {
        registry.ctx().get<entt::dispatcher>().sink<events::Resize>().connect<&Renderer::handle_resize>(*this);
        auto &camera = registry.ctx().get<Camera>();
        auto size = window.getSize();
        set_size(size);
        camera.resize(glm::vec2(size.x, size.y));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        lights.bind_to(UniformBindings::Lights);
    }

    void Renderer::operator()(const sf::Window &window, entt::registry &registry) {
        auto &camera = registry.ctx().get<Camera>();
        auto &controller = registry.ctx().get<components::PanOrbitCamera>();
        Color clear_color;
        if (registry.ctx().contains<ClearColor>()) clear_color = registry.ctx().get<ClearColor>().color;
        else
            clear_color = registry.ctx().insert_or_assign(ClearColor{Color::Black}).color;
        auto drawables = registry.view<const Transform, const Mesh, const ProgramRef>();

        if (size_changed) {
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

        const_cast<sf::Window &>(window).display();
    }
}// namespace shell::gl