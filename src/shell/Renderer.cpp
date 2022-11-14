//
// Created by solarliner on 12/11/22.
//

#include "Renderer.h"
#include "Color.h"
#include "gl/Camera.h"
#include "gl/ClearColor.h"
#include "gl/Mesh.h"
#include "gl/ProgramRef.h"
#include "gl/Transform.h"
#include "gl/resource/shaders/Program.h"
#include "glm/gtx/string_cast.hpp"

static void set_size(GLint w, GLint h) { glViewport(0, 0, w, h); }
static void set_size(unsigned int w, unsigned int h) { set_size((GLint) w, (GLint) h); }
static void set_size(sf::Vector2u size) { set_size(size.x, size.y); }
static void set_size(glm::vec2 size) { set_size((GLint) size.x, (GLint) size.y); }

namespace shell::gl {
    void Renderer::handle_resize(events::Resize resize) { size_changed = glm::vec2(resize.width, resize.height); }

    void Renderer::before_run(const sf::Window &window, entt::registry &registry) {
        registry.ctx().get<entt::dispatcher>().sink<events::Resize>().connect<&Renderer::handle_resize>(*this);
        auto &camera = registry.ctx().get<Camera>();
        auto size = window.getSize();
        set_size(size);
        camera.resize(size.x, size.y);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void Renderer::operator()(const sf::Window &window, entt::registry &registry) {
        auto &camera = registry.ctx().get<Camera>();
        Color clear_color;
        if (registry.ctx().contains<ClearColor>()) clear_color = registry.ctx().get<ClearColor>().color;
        else
            clear_color = registry.ctx().insert_or_assign(ClearColor{Color::Black}).color;
        auto drawables = registry.view<const Transform, const Mesh, const ProgramRef>();

        if (size_changed) {
            camera.resize(size_changed->x, size_changed->y);
            set_size(*size_changed);
            size_changed.reset();
        }

        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClearDepth(1.f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto [entity, model, mesh, material]: drawables.each()) {
            const auto &program = material.get_program();
            program.use();
            program.set_uniform("model", model.matrix);
            program.set_uniform("view", camera.view());
            program.set_uniform("projection", camera.projection());
            mesh.draw(registry.any_of<wireframe>(entity));
        }

        const_cast<sf::Window &>(window).display();
    }
}// namespace shell::gl