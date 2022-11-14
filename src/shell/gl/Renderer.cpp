//
// Created by solarliner on 12/11/22.
//

#include "Renderer.h"
#include "Camera.h"
#include "ClearColor.h"
#include "Mesh.h"
#include "Transform.h"
#include "resource/shaders/Program.h"
#include <glm/gtx/string_cast.hpp>

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
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }

    void Renderer::operator()(const sf::Window &window, entt::registry &registry) {
        auto camera = registry.ctx().get<Camera>();
        Color clear_color;
        if (registry.ctx().contains<ClearColor>()) clear_color = registry.ctx().get<ClearColor>().color;
        else
            clear_color = registry.ctx().insert_or_assign(ClearColor{Color::Black}).color;
        auto drawables = registry.view<const Transform, const Mesh, const Program>();

        if (size_changed) {
            std::cout << "Size changed " << glm::to_string(*size_changed) << std::endl;
            camera.resize(size_changed->x, size_changed->y);
            set_size(*size_changed);
            size_changed.reset();
        }

        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClearDepth(1.f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto [_, model, mesh, material]: drawables.each()) {
            material.use();
            material.set_uniform("model", model.matrix);
            material.set_uniform("view", camera.view());
            material.set_uniform("projection", camera.projection());
            mesh.draw();
        }

        const_cast<sf::Window &>(window).display();
    }
}// namespace shell::gl